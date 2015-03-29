Drupal.style_manager.import_css_code = function (id) {
  if (Drupal.style_manager.import_css_code_win) {
    Drupal.style_manager.import_css_code_win.destroy();
  }

  var check_box_items = [];
  $.each(Drupal.settings.ajaxPageState.css, function (index, value) {
    if (index.indexOf('themes/') != -1) {
      var checked = true;
      if (index.indexOf('/ie') != -1 || index.indexOf('print') != -1) {
        checked = false;
      }
      check_box_items.push({
        checked: checked,
        boxLabel: index,
        name: index
      });
    }
  });
  $.each(Drupal.settings.ajaxPageState.css, function (index, value) {
    if (index.indexOf('/themes/') == -1) {
      check_box_items.push({
        boxLabel: index,
        name: index
      });
    }
  });

  Drupal.style_manager.import_css_code_win = Ext.create('Ext.window.Window', {
    title: 'Import from css files',
    id: 'import_css_code_win',
    textarea_id: id,
    layout: 'fit',
    hidden: false,
    collapsible: false,
    stateful: false,
    width: 1065,
    height: 500,
    resizable: false,
    items: [
      {
        xtype: 'fieldcontainer',
        layout: 'hbox',
        autoScroll: true,
        fieldDefaults: {
          labelAlign: 'top'
        },
        items: [
          {
            labelStyle: 'font-weight:bold',
            id: 'import_css_code_checkbox',
            xtype: 'checkboxgroup',
            margins: "10px",
            flex: 1,
            fieldLabel: 'Select styles for import',
            columns: 1,
            items: check_box_items
          }
        ]
      }
    ],
    buttons: [
      {
        text: 'Import images/color property',
        width: 170,
        handler: function () {
          var post_data = {
            type: 'image_color',
            styles: Ext.getCmp('import_css_code_checkbox').getValue(),
            token: Drupal.settings.style_manager.token
          };
          jQuery.post(Drupal.settings.basePath + 'style_manager/css_widget_import_css.json', post_data, function (data) {
            var win = Ext.getCmp('import_css_code_win');
            Ext.getCmp(win.textarea_id).setValue(data.css);
            win.hide();
          }, 'json');
        }
      },
      {
        text: 'Import all',
        width: 85,
        handler: function () {
          var post_data = {
            type: 'all',
            styles: Ext.getCmp('import_css_code_checkbox').getValue(),
            token: Drupal.settings.style_manager.token
          };
          jQuery.post(Drupal.settings.basePath + 'style_manager/css_widget_import_css.json', post_data, function (data) {
            var win = Ext.getCmp('import_css_code_win');
            Ext.getCmp(win.textarea_id).setValue(data.css);
            win.hide();
          }, 'json');
        }
      },
      '-',
      {
        text: 'Cancel',
        width: 85,
        handler: function () {
          this.up('window').hide();
        }
      }
    ]
  });
  var parentWinPosition = Drupal.style_manager.win.getPosition();
  Drupal.style_manager.import_css_code_win.setPosition(parentWinPosition[0], parentWinPosition[1]);
  Drupal.style_manager.import_css_code_win.show();
}
