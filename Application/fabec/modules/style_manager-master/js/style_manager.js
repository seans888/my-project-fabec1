Drupal.style_manager = {
  'id': 'style_manager_class',
  'advanced_numeric_store': {},
  'gradient_presets': {},
  'hidden_win': {},
  'btn_autosave_pressed': true,
  'loop_time': 500,
  'previous_selected_cat': '',
  'previous_selected_preset': '',
  'page_styles_loading': false,
  'page_styles_need_upd': false,

  'add_style_settings_btn': function () {
    jQuery('body').append('<a id="style_settings_btn" href="#"; ></a>');
    jQuery('#style_settings_btn').click(function () {
      if (Drupal.style_manager.win.isVisible()) {
        Drupal.style_manager.win_hide(true);
      }
      else {
        Drupal.style_manager.win_show();
      }
      return false;
    });
    jQuery('#style_settings_btn').hover(function () {
        Drupal.style_manager.settings_btn_animate_hide = false;
        jQuery(this).stop().animate({width: '30px'}, {duration: 100});
      },
      function () {
        Drupal.style_manager.settings_btn_animate_hide = true;
        setTimeout(function () {
          if (Drupal.style_manager.settings_btn_animate_hide && !Drupal.style_manager.win.isVisible()) {
            jQuery('#style_settings_btn').stop().animate({width: "10px"}, {duration: 100});
            Drupal.style_manager.settings_btn_animate_hide = false;
          }
        }, 1000);
      });
  },

  'win_show': function () {
    if (Drupal.style_manager.winInit) {
      Drupal.style_manager.win.center();
      Drupal.style_manager.win.setPosition(($(document).width() - 1120));
      Drupal.style_manager.win.show();
    }
    else {
      Drupal.style_manager.win.setPosition(($(document).width() - 1120));
      Drupal.style_manager.win.show('style_settings_btn');
    }
    Drupal.style_manager.winInit = true;
    var parentWinPosition = Drupal.style_manager.win.getPosition();
    if (Drupal.style_manager.hidden_win.cssInspectorWin) {
      Drupal.style_manager.cssInspectorWin.setPosition(parentWinPosition[0], parentWinPosition[1]);
      Drupal.style_manager.cssInspectorWin.show();
    }
    if (Drupal.style_manager.hidden_win.output_css_win) {
      Drupal.style_manager.output_css_win.setPosition(parentWinPosition[0] - 400, parentWinPosition[1]);
      Drupal.style_manager.output_css_win.show();
    }
    if (Drupal.style_manager.hidden_win.settings_win) {
      Drupal.style_manager.settings_win.setPosition(parentWinPosition[0], parentWinPosition[1]);
      Drupal.style_manager.settings_win.show();
    }
    Drupal.style_manager.hidden_win = {};
  },

  'win_hide': function (hide_main_win) {
    if (hide_main_win) {
      Drupal.style_manager.win.hide();
    }
    if (Drupal.style_manager.cssInspectorWin && Drupal.style_manager.cssInspectorWin.isVisible()) {
      Drupal.style_manager.hidden_win.cssInspectorWin = true;
      Drupal.style_manager.cssInspectorWin.hide();
    }
    if (Drupal.style_manager.output_css_win && Drupal.style_manager.output_css_win.isVisible()) {
      Drupal.style_manager.hidden_win.output_css_win = true;
      Drupal.style_manager.output_css_win.hide();
    }
    if (Drupal.style_manager.settings_win && Drupal.style_manager.settings_win.isVisible()) {
      Drupal.style_manager.hidden_win.settings_win = true;
      Drupal.style_manager.settings_win.hide();
    }
  },

  'colorpicker': function () {
    jQuery('.colorpickerfield input').not('.colorpicker_processed').addClass('colorpicker_processed').each(function () {
      new jscolor.color(jQuery(this)[0], {required: false, pickerZIndex: 30000, pickerFaceColor: '#dbe3ed'});
    });
  },

  'ext_fieldset': function () {
    jQuery('.x-fieldset-header-text').not('.processed').addClass('processed').click(function () {
      var fieldset_id = jQuery(this).parents('fieldset:first').attr('id');
      Ext.getCmp(fieldset_id).toggle();
    });
  },

  'get_form': function (cat_id) {
    Drupal.style_manager.form_loading(true);
    var post_data = {
      cat: cat_id,
      group: Drupal.style_manager.selectrd_group,
      token: Drupal.settings.style_manager.token
    };
    jQuery.post(Drupal.settings.basePath + 'style_manager/get_styles_form.json', post_data, function (data) {
      var form_obj = eval('(' + data + ')');
      Drupal.style_manager.replace_form(form_obj);
    });
  },

  'show_alerts': function (data) {
    if (data['message']) {
      alert(data['message']);
    }
  },

  'preset_item_visibility_settings_upd': function (item, checked){
    var data = {};
    data.cat = Drupal.style_manager.selected_cat;
    data.group = Drupal.style_manager.selectrd_group;
    data.token = Drupal.settings.style_manager.token;
    if (item.global) {
      if (!checked) {
        return;
      }
      Drupal.style_manager.form_loading(true);
      Drupal.style_manager.preset_item_visibility_changed = false;
      data.key = 'visibility_type';
      data.value = item.id;
    }
    else {
      data.key = item.id;
      data.value = checked;
      Drupal.style_manager.preset_item_visibility_changed = true;
    }
    jQuery.post(Drupal.settings.basePath + 'style_manager/preset_item_visibility_settings_save.json', data, function (data) {
      if (data.key == 'visibility_type') {
        Drupal.style_manager.get_form(data.cat_id);
        Drupal.style_manager.upd_page_styles(data.cat_id);
      }
    }, 'json');
  },

  'replace_form': function (data) {
    if (data.cat_id == Drupal.style_manager.selected_cat) {
      Drupal.style_manager.original_form_data = data;
      Drupal.style_manager.original_menu_data = [];
      jQuery.each(data.presets, function (index, value) {
        if (Drupal.style_manager.selected_cat != value.id && value.id.split('--')[1] != 'disable') {
          Drupal.style_manager.original_menu_data.push({
              text: 'Load from: ' + value.text,
              iconCls: 'reset_from_icon',
              handler: function () {
                Drupal.style_manager.reset_form = true;
                Drupal.style_manager.form_loading(true);
                Drupal.style_manager.get_form(value.id);
              }}
          );
        }
      });
    }
    Drupal.style_manager.preset_item_visibility_changed = false;
    Drupal.style_manager.elements_menu = [];
    jQuery.each(data.visibility_settings, function (index, value) {
      value.checkHandler = Drupal.style_manager.preset_item_visibility_settings_upd;
      value.global = false;
      Drupal.style_manager.elements_menu.push(value);
    });
    if (Drupal.style_manager.reset_form) {
      var visibility_settings_arr = {};
      jQuery.each(data.visibility_settings, function (index, value) {
        visibility_settings_arr[value.id] = value.checked;
      });
      visibility_settings_arr.visibility_type = data.visibility_type;
      var post_data = {};
      post_data.cat = Drupal.style_manager.selected_cat;
      post_data.group = Drupal.style_manager.selectrd_group;
      post_data.visibility_settings = Ext.JSON.encode(visibility_settings_arr);
      post_data.token = Drupal.settings.style_manager.token;
      jQuery.post(Drupal.settings.basePath + 'style_manager/preset_item_visibility_settings_save_all.json', post_data, function (data) {
      }, 'json');
    }
    Drupal.style_manager.reset_form = false;
    var formPanel = Ext.create('Ext.form.Panel', {
      autoScroll: true,
      id: 'style_manager_form',
      frame: true,
      stateful: false,
      height: 440,
      defaults: { width: 645, stateful: false },
      items: data.form,

      bbar: [
        {
          iconCls: data.visibility_type,
          id: 'style_manager_preset_settings',
          menu: {
            xtype: 'menu',
            stateful: false,
            items: [
              {
                text: 'Show all',
                id: 'show_all',
                global: true,
                checked: (data.visibility_type == 'show_all'),
                checkHandler: Drupal.style_manager.preset_item_visibility_settings_upd,
                group: 'elements_menu'
              },
              {
                text: 'Show selected',
                id: 'show_selected',
                global: true,
                checked: (data.visibility_type == 'show_selected'),
                checkHandler: Drupal.style_manager.preset_item_visibility_settings_upd,
                group: 'elements_menu'
              },
              {
                text: 'Hide selected',
                id: 'hide_selected',
                global: true,
                checked: (data.visibility_type == 'hide_selected'),
                checkHandler: Drupal.style_manager.preset_item_visibility_settings_upd,
                group: 'elements_menu'
              }, '-', {
                text: 'Selected elements',
                menu: {
                  items: Drupal.style_manager.elements_menu
                }
              }
            ],
            listeners: {
              beforehide: function () {
                if (Drupal.style_manager.preset_item_visibility_changed) {
                  Drupal.style_manager.form_loading(true);
                  Drupal.style_manager.get_form(Drupal.style_manager.selected_cat);
                }
              }
            }
          }
        },
        '->',
        {
          text: 'Reset',
          xtype: 'splitbutton',
          minWidth: 85,
          id: 'style_manager_btn_reset_form',
          handler: function () {
            Drupal.style_manager.reset_form = true;
            Drupal.style_manager.form_loading(true);
            Drupal.style_manager.replace_form(Drupal.style_manager.original_form_data);
          },
          stateful: false,
          menu: new Ext.menu.Menu({
            items: Drupal.style_manager.original_menu_data
          })
        },
        '-',
        {
          text: 'Save',
          minWidth: 85,
          id: 'style_manager_btn_save_form',
          handler: Drupal.style_manager.save_form,
          stateful: false
        },
        {
          text: 'Autosave',
          minWidth: 85,
          id: 'style_manager_btn_autosave_form',
          stateful: false,
          enableToggle: true,
          toggleHandler: function (item, pressed) {
            Drupal.style_manager.btn_autosave_pressed = pressed;
          },
          pressed: Drupal.style_manager.btn_autosave_pressed
        }
      ]
    });
    Ext.getCmp('style_manager_el_form').removeAll();
    Ext.getCmp('style_manager_el_form').add(formPanel);

    Drupal.style_manager.form_loading(false);
  },

  'save_form': function () {
    var form = Ext.getCmp('style_manager_form').getForm().getFieldValues();
    jQuery.each(form, function (index, value) {
      if (index.substring(0, 7) == 'ext-gen') {
        delete form[index];
      }
    });
    delete form['type'];
    if (!Object.keys(form).length) {
      return;
    }
    if (this.id && this.id == 'style_manager_class') {
      var post_data = {
        field_values: Ext.JSON.encode(form),
        cat: Drupal.style_manager.selected_cat,
        token: Drupal.settings.style_manager.token
      };
      jQuery.post(Drupal.settings.basePath + 'style_manager/save_styles.json', post_data, function (data) {
        Drupal.style_manager.upd_page_styles();
        Drupal.style_manager.show_alerts(data);
      }, 'json');
    }
    else {
      Drupal.style_manager.form_loading(true);
      var post_data = {
        field_values: Ext.JSON.encode(form),
        cat: Drupal.style_manager.selected_cat,
        token: Drupal.settings.style_manager.token
      };
      jQuery.post(Drupal.settings.basePath + 'style_manager/save_styles.json', post_data, function (data) {
        Drupal.style_manager.upd_page_styles();
        Drupal.style_manager.show_alerts(data);
        Drupal.style_manager.form_loading(false);
      }, 'json');
    }
  },

  'loop': function () {
    var btn = Ext.getCmp('style_manager_btn_autosave_form');
    var save_btn = Ext.getCmp('style_manager_btn_save_form');

    if (!Drupal.style_manager.page_styles_loading && Drupal.style_manager.page_styles_need_upd) {
      Drupal.style_manager.upd_page_styles(Drupal.style_manager.page_styles_upd_cat);
      Drupal.style_manager.page_styles_need_upd = false;
      Drupal.style_manager.page_styles_upd_cat = false;
    }

    var save_btn_disabled = false;
    if (btn && btn.disabled === false && btn.pressed === true) {
      save_btn_disabled = true;
      var form = Ext.JSON.encode(Ext.getCmp('style_manager_form').getForm().getFieldValues());
      if (Drupal.style_manager.last_cat != Drupal.style_manager.selected_cat) {
        Drupal.style_manager.last_autosave_data = form;
        Drupal.style_manager.last_cat = Drupal.style_manager.selected_cat;
      }
      else if (form != Drupal.style_manager.last_autosave_data) {
        Drupal.style_manager.save_form();
        Drupal.style_manager.last_autosave_data = form;
      }
    }

    if (save_btn) {
      save_btn.setDisabled(save_btn_disabled);
    }
    Drupal.style_manager.colorpicker();
    Drupal.style_manager.ext_fieldset();
    Drupal.style_manager.preset_menu_init();
    Drupal.style_manager.category_menu_init();
    setTimeout(Drupal.style_manager.loop, Drupal.style_manager.loop_time);

  },

  'form_loading': function (loading) {
    if (loading) {
      if (Ext.getCmp('style_manager_btn_save_form')) {
        Ext.getCmp('style_manager_btn_save_form').setDisabled(true);
        Ext.getCmp('style_manager_btn_reset_form').setDisabled(true);
        Ext.getCmp('style_manager_btn_autosave_form').setDisabled(true);
      }
      Drupal.style_manager.loadMask.show();
    }
    else {
      if (Ext.getCmp('style_manager_btn_save_form')) {
        Ext.getCmp('style_manager_btn_save_form').setDisabled(false);
        Ext.getCmp('style_manager_btn_reset_form').setDisabled(false);
        Ext.getCmp('style_manager_btn_autosave_form').setDisabled(false);
      }
      Drupal.style_manager.loadMask.hide();
    }
  },

  'preset_save_as': function (selected_cat) {
    Ext.MessageBox.prompt('Save preset as', 'Preset name:', function (btn, text) {
      if (btn == 'ok' && text != '') {
        var post_data = {
          cat: selected_cat,
          title: text,
          token: Drupal.settings.style_manager.token
        };
        jQuery.post(Drupal.settings.basePath + 'style_manager/preset_save_as.json', post_data, function (data) {
          Drupal.style_manager.filter_category_tree_load(data.old_id);
          Drupal.style_manager.show_alerts(data);
        }, 'json');
      }
      if (btn == 'ok' && text == '') {
        Drupal.style_manager.preset_save_as();
      }
    });
  },

  'preset_delete': function (selected_cat) {
    Ext.MessageBox.confirm('Confirm', 'Are you sure you want to delete selectrd preset?', function (btn, text) {
      if (btn == 'yes' && text != '') {
        var post_data = {
          cat: selected_cat,
          token: Drupal.settings.style_manager.token
        };
        jQuery.post(Drupal.settings.basePath + 'style_manager/preset_delete.json', post_data, function (data) {
          Drupal.style_manager.filter_category_tree_load(data.cat_id);
          Drupal.style_manager.show_alerts(data);
        }, 'json');
      }
    });
  },

  'preset_rename': function (selected_cat) {
    Ext.MessageBox.prompt('Rename preset', 'New name:', function (btn, text) {
      if (btn == 'ok' && text != '') {
        var post_data = {
          cat: selected_cat,
          title: text,
          token: Drupal.settings.style_manager.token
        };
        jQuery.post(Drupal.settings.basePath + 'style_manager/preset_rename.json', post_data, function (data) {
          Drupal.style_manager.filter_category_tree_load(data.new_id);
          Drupal.style_manager.show_alerts(data);
        }, 'json');
      }
      if (btn == 'ok' && text == '') {
        Drupal.style_manager.preset_rename();
      }
    });
  },

  'filter_category_tree_load': function (new_id) {
    Drupal.style_manager.new_tree_id = new_id;
    var cat = '';
    if (Drupal.style_manager.selected_cat_arr) {
      cat = Drupal.style_manager.selected_cat_arr[0];
    }
    var post_data = {
      selected_cat: cat,
      token: Drupal.settings.style_manager.token
    };
    jQuery.post(Drupal.settings.basePath + 'style_manager/get_cat_list.json', post_data, function (data) {
      var root = Drupal.style_manager.catTree.getRootNode();
      root.removeAll();
      root.appendChild(data);
      setTimeout(Drupal.style_manager.select_in_preset_tree, 100);
    }, 'json');
  },

  'set_default_preset': function (selected_cat) {
    var post_data = {
      cat: selected_cat,
      token: Drupal.settings.style_manager.token
    };
    jQuery.post(Drupal.settings.basePath + 'style_manager/set_default_preset.json', post_data, function (data) {
      Drupal.style_manager.filter_category_tree_load(Drupal.style_manager.selected_cat);
    }, 'json');
  },

  'select_in_preset_tree': function () {
    Drupal.style_manager.previous_selected_cat = '';
    Drupal.style_manager.previous_selected_preset = '';
    Drupal.style_manager.catTree.getSelectionModel().select(Drupal.style_manager.catStore.getNodeById(Drupal.style_manager.new_tree_id), true);
  },

  'upd_page_styles': function (selected_cat) {
    if (Drupal.style_manager.page_styles_loading) {
      Drupal.style_manager.page_styles_need_upd = true;
      Drupal.style_manager.page_styles_upd_cat = selected_cat;
      return;
    }
    Drupal.style_manager.page_styles_loading = true;
    if (!Drupal.style_manager.selected_cat && !selected_cat) {
      selected_cat = '';
    }
    else if (!selected_cat){
      selected_cat = Drupal.style_manager.selected_cat;
    }
    var post_data = {
      cat: selected_cat,
      token: Drupal.settings.style_manager.token
    };
    Drupal.style_manager.win.setIconCls('hourglass');
    Drupal.style_manager.win.setTitle('Loading css...');
    jQuery.post(Drupal.settings.basePath + 'style_manager/get_css.json', post_data, function (data) {
      if (data.reset_all) {
        jQuery('.style_manager_preview_styles').remove();
      }
      jQuery(data.data).each(function (key, cat_data) {
        jQuery('#style_manager_cat_' + cat_data.cat).remove();
        jQuery('body').append('<style class="style_manager_preview_styles" id="style_manager_cat_' + cat_data.cat + '">' + cat_data.css + '</style>');
      });
      Drupal.style_manager.upd_page_output_css_textarea();
      Drupal.style_manager.page_styles_loading = false;
      Drupal.style_manager.win.setIconCls('status');
      Drupal.style_manager.win.setTitle(Drupal.settings.style_manager.title);

    }, 'json');
  },

  'imce_browser_open': function (wrapper_id) {
    var input_id = jQuery('#' + wrapper_id + ' .x-form-field:first').attr('id');
    window.open(Drupal.settings.style_manager.imce.url + encodeURIComponent(input_id), '', 'width=760,height=560,resizable=1');
  },

  'upd_page_output_css_textarea': function () {
    var css_code = '';
    jQuery('.style_manager_preview_styles').each(function () {
      css_code += jQuery(this).html();
    });

    jQuery('#style_manager_output_css').html(css_code);
    jQuery('#style_manager_output_css').syntaxCSS();
  },

  'preset_menu_init': function () {
    jQuery('.preset_menu').not('.processed').addClass('processed').click(function () {
      var id = jQuery(this).attr('data-id');
      var cat_arr = id.split('--');
      var contextMenu = Ext.create('Ext.menu.Menu', {
        focusOnToFront: false,
        items: [
          {
            text: 'Default',
            iconCls: 'act_preset_icon',
            id: 'style_manager_btn_default_preset',
            handler: function() { Drupal.style_manager.set_default_preset(id); },
            disabled: (Drupal.style_manager.selected_cat_data && Drupal.style_manager.selected_cat_data.raw['default'])
          }, '-',
          {
            text: 'Clone preset',
            iconCls: 'clone_icon',
            id: 'style_manager_btn_save_preset_as',
            handler: function() { Drupal.style_manager.preset_save_as(id); },
            disabled: (cat_arr[1] == 'disable')
          },
          {
            text: 'Rename',
            iconCls: 'rename_icon',
            id: 'style_manager_btn_preset_rename',
            handler: function() { Drupal.style_manager.preset_rename(id); },
            disabled: (cat_arr[1] == 'disable' || cat_arr[1] == 'default')
          },
          {
            text: 'Delete',
            iconCls: 'delete_icon',
            id: 'style_manager_btn_delete_preset',
            handler: function() { Drupal.style_manager.preset_delete(id); },
            disabled: (cat_arr[1] == 'disable' || cat_arr[1] == 'default')
          }
        ]
      });
      var position = jQuery(this).offset();
      contextMenu.showAt(position.left, position.top + 15);
      return false;
    });
  },

  'category_menu_init': function () {
    jQuery('.category_menu').not('.processed').addClass('processed').click(function () {
      var id = jQuery(this).attr('data-id');
      style_manager_settings(id);
      return false;
    });
  }
}
