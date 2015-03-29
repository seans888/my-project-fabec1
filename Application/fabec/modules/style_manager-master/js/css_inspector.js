Ext.onReady(function () {
  $ = jQuery;

  function addCssCodeTo(el_id, css_code, highlight) {
    css_code = css_code.replace(/{/gi, "\n{\n  ");
    css_code = css_code.replace(/;/gi, ";\n  ");
    css_code = css_code.replace(/  }/gi, "}\n\n");
    $(el_id).html(css_code);
    $(el_id).syntaxCSS();

    if (highlight) {
      var content = $(el_id).html();
      if (content) {
        var highlight = highlight
          .replace(/\{/gi, ':')
          .replace(/\}/gi, ':')
          .replace(/\;/gi, ':');
        highlight_arr = highlight.split(':');
        $.each(highlight_arr, function (index, value) {
          value = $.trim(value);
          content = content.split(value).join('<span class="css_code_highlight" ">' + value + '</span>');
        });
        $(el_id).html(content);
      }
    }

    $(el_id + ' .sel').click(function () {
      var resultSelector = Ext.getCmp('style_manager_css_selector').getValue();
      var newSelector = $.trim($(this).text());

      if ($.trim(resultSelector) != '') {
        var newSelectorArr = newSelector.split(',');
        var resultSelectorArr = resultSelector.split(',');
        var resultUniqueSelectorArr = [];

        $.each(resultSelectorArr, function (index, value) {
          resultSelectorArr[index] = $.trim(value);
        });
        $.each(newSelectorArr, function (index, value) {
          value = $.trim(value);
          if ($.inArray(value, resultSelectorArr) == -1) {
            resultUniqueSelectorArr.push(value);
          }
        });
        newSelector = $.trim(resultUniqueSelectorArr.join(', '));
        if (newSelector != '') {
          newSelector = newSelector + ",\n";
        }
      }
      resultSelector = newSelector + resultSelector;
      Ext.getCmp('style_manager_css_selector').setValue(resultSelector);
    });
  }

  function searchInCss(search_text) {
    search_text = $.trim(search_text);
    var styles_arr = searchInCssGetStylesList(search_text);
    var styles_text = '';

    $.each(styles_arr, function (index, value) {
      if (value.cssText) {
        styles_text = styles_text + value.cssText;
      }
      if (index == 500) {
        styles_text = styles_text + "\n/* ... */\n\n";
        addCssCodeTo('#css_inspector_search', styles_text, search_text);
        return false;
      }
    });
    addCssCodeTo('#css_inspector_search', styles_text, search_text);
    return false;
  }

  function searchInCssGetStylesList(search_text, style_in) {
    style_in = document.styleSheets;
    var style_out = [];

    $.each(style_in, function (index, value) {
      if (value.cssRules) {
        $.each(value.cssRules, function (index2, value2) {
          if (value2.styleSheet && value2.styleSheet.rules) {
            $.each(value2.styleSheet.rules, function (index3, value3) {
              if (!value3.styleSheet && value3.cssText && value3.cssText.indexOf(search_text) != -1) {
                style_out.push(value3);
              }
            });
          }
          else if (value2.cssText && value2.cssText.indexOf(search_text) != -1) {
            style_out.push(value2);
          }
        });
      }
    });
    return style_out;
  }

  function showCssFromEl(el) {
    if (window.getMatchedCSSRules) {
      var css_code = '';
      var rules = window.getMatchedCSSRules(el);
      $.each(rules, function (index, value) {
        if (value.selectorText != '.css_inspector_hover') {
          css_code = value.cssText + css_code;
        }
      });
      addCssCodeTo('#css_inspector_inspect', css_code);
    }
    else {
      $('#css_inspector_inspect').html('Your browser is not supported.');
    }
  };

  function inspectStart() {
    $('.css_inspector_hover').removeClass('css_inspector_hover');
    $(document).bind({
      mouseover: function (event) {
        if ($(event.target).parents('.x-window-item')[0]) {
          return false;
        }
        var elText = $('<div>').append($(event.target).clone().html('')).html();
        $(event.target).addClass('css_inspector_hover');
        $('#css_inspector_selected_el').text(elText);
      },
      mouseout: function (event) {
        $(event.target).removeClass('css_inspector_hover');
      },
      click: function (event) {
        if (!$(event.target).parents('.x-window-item')[0]) {
          if ($(event.target).hasClass('css_inspector_hover')) {
            Ext.getCmp('style_manager_btn_css_inspector_inspect').toggle(false);
            inspectEnd();
            showCssFromEl(event.target);
          }
          return false;
        }
      }
    });
  }

  function inspectEnd() {
    $(document).unbind('mouseover');
    $(document).unbind('mouseout');
    $(document).unbind('click');
  }

  Drupal.style_manager.showCssInspectorWin = function() {
    if (!Drupal.style_manager.cssInspectorWin) {
      var form = Ext.widget('form', {
        layout: {
          type: 'vbox',
          align: 'stretch'
        },
        frame: true,
        border: false,
        items: [
          {
            fieldLabel: 'Selector',
            xtype: 'textareafield',
            labelAlign: 'top',
            id: 'style_manager_css_selector',
            height: 100,
            margins: 0,
            allowBlank: true
          },
          {
            xtype: 'tabpanel',
            id: 'style_manager_css_inspector_tabpanel',
            plain: true,
            defaults: {
              bodyPadding: 0
            },
            flex: 2,
            items: [
              {
                title: "Inspect element",
                id: "style_manager_css_inspector_tabpanel_inspect",
                layout: {
                  type: 'vbox',
                  align: 'stretch'
                },
                frame: true,
                items: [
                  {
                    xtype: 'fieldcontainer',
                    layout: 'hbox',
                    items: [
                      {
                        xtype: 'displayfield',
                        value: '<div id="css_inspector_selected_el"></div>',
                        flex: 1
                      },
                      {
                        text: 'Inspect ',
                        xtype: 'button',
                        height: 25,
                        width: 80,
                        iconCls: 'bug',
                        id: 'style_manager_btn_css_inspector_inspect',
                        stateful: false,
                        enableToggle: true,
                        toggleHandler: function (item, pressed) {
                          if (pressed) {
                            inspectStart();
                          }
                          else {
                            inspectEnd();
                          }
                        }
                      }
                    ]
                  },
                  {
                    xtype: 'displayfield',
                    value: '<div class="css_inspector_info" id="css_inspector_inspect"></div>',
                    flex: 1
                  }
                ]
              },
              {
                title: "Search",
                id: "style_manager_css_inspector_tabpanel_search",
                layout: {
                  type: 'vbox',
                  align: 'stretch'
                },
                frame: true,
                items: [
                  {
                    xtype: 'fieldcontainer',
                    layout: 'hbox',
                    items: [
                      {
                        xtype: 'textfield',
                        id: 'css_inspector_search_text',
                        allowBlank: true,
                        listeners: {
                          specialkey: function (f, e) {
                            if (e.getKey() == e.ENTER) {
                              searchInCss(Ext.getCmp('css_inspector_search_text').getValue());
                            }
                          }
                        },
                        margins: "2px 5px 0 0",
                        flex: 1
                      },
                      {
                        text: 'Search',
                        xtype: 'button',
                        height: 25,
                        width: 80,
                        id: 'style_manager_btn_css_inspector_search',
                        handler: function () {
                          searchInCss(Ext.getCmp('css_inspector_search_text').getValue());
                        }
                      }
                    ]
                  },
                  {
                    xtype: 'displayfield',
                    value: '<div class="css_inspector_info" id="css_inspector_search"></div>',
                    flex: 1
                  }
                ]
              }
            ]
          }
        ],

        buttons: [
          {
            text: 'Add all selectors',
            width: 120,
            id: 'style_manager_btn_css_inspector_add_all',
            handler: function () {
              if (Ext.getCmp('style_manager_css_inspector_tabpanel').getActiveTab().id == 'style_manager_css_inspector_tabpanel_inspect') {
                $('#css_inspector_inspect .sel').each(function(){
                  $(this).click();
                });
              }
              if (Ext.getCmp('style_manager_css_inspector_tabpanel').getActiveTab().id == 'style_manager_css_inspector_tabpanel_search') {
                $('#css_inspector_search .sel').each(function(){
                  $(this).click();
                });
              }
            }
          }, '-',
          {
            text: 'Ok',
            id: 'style_manager_btn_css_inspector_ok',
            handler: function () {
              if (Drupal.style_manager.cssInspectorParentTextfield) {
                Drupal.style_manager.cssInspectorParentTextfield.setValue(Ext.getCmp('style_manager_css_selector').getValue());
              }
              this.up('form').getForm().reset();
              this.up('window').hide();
            }
          },
          {
            text: 'Cancel',
            handler: function () {
              this.up('window').hide();
            }
          }
        ]
      });

      Drupal.style_manager.cssInspectorWin = Ext.widget('window', {
        title: 'CSS inspector',
        closeAction: 'hide',
        width: 1065,
        height: 500,
        minHeight: 300,
        layout: 'fit',
        resizable: true,
        listeners: {
          hide: function () {
            $('.css_inspector_hover').removeClass('css_inspector_hover');
            inspectEnd();
          }
        },
        modal: true,
        items: form
      });
    }

    var parentWinPosition = Drupal.style_manager.win.getPosition();
    Drupal.style_manager.cssInspectorWin.setPosition(parentWinPosition[0], parentWinPosition[1]);
    Drupal.style_manager.cssInspectorWin.show();
    if (Drupal.style_manager.cssInspectorParentTextfield) {
      Ext.getCmp('style_manager_css_selector').setValue(Drupal.style_manager.cssInspectorParentTextfield.getValue());
      Ext.getCmp('style_manager_btn_css_inspector_ok').show();
      Ext.getCmp('style_manager_btn_css_inspector_add_all').show();
      Ext.getCmp('style_manager_css_selector').show();
      Ext.getCmp('style_manager_btn_css_inspector_inspect').toggle(true);
      Ext.getCmp('style_manager_css_inspector_tabpanel').setActiveTab('style_manager_css_inspector_tabpanel_inspect');
    }
    else {
      Ext.getCmp('style_manager_css_selector').setValue('');
      Ext.getCmp('style_manager_btn_css_inspector_ok').hide(true);
      Ext.getCmp('style_manager_btn_css_inspector_add_all').hide(true);
      Ext.getCmp('style_manager_css_selector').hide(true);
      Ext.getCmp('style_manager_btn_css_inspector_inspect').toggle(false);
      Ext.getCmp('style_manager_css_inspector_tabpanel').setActiveTab('style_manager_css_inspector_tabpanel_inspect');
    }
  };

  Ext.define('Ext.ux.CssInspector', {
    extend: 'Ext.form.field.Trigger',
    alias: 'widget.cssinspector',
    initComponent: function () {
      var me = this;

      me.triggerCls = 'x-form-search-trigger';

      me.callParent(arguments);
    },
    // override onTriggerClick
    onTriggerClick: function () {
      Drupal.style_manager.cssInspectorParentTextfield = this;
      Drupal.style_manager.showCssInspectorWin();
    }
  });

});
