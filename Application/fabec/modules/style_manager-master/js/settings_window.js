var style_manager_settings = function (cat_id, act_tab) {
  Drupal.style_manager.style_manager_settings_open_groups = {};
  Drupal.style_manager.style_manager_settings_open_groups[Drupal.style_manager.selectrd_group] = true;
  if (!act_tab) {
    act_tab = 0;
  }
  if (Drupal.style_manager.settings_win) {
    Drupal.style_manager.settings_win.destroy();
  }
  if (Drupal.style_manager.settings_group_win) {
    Drupal.style_manager.settings_group_win.destroy();
  }
  Ext.define('settings_model', {
    extend: 'Ext.data.Model',
    fields: ['id', 'weight', 'group', 'title', 'type', 'selector', 'locked']
  });

  var updInfo = function (cat_id) {
    loadMask.show();
    Drupal.style_manager.settings_win_store.load();
    if (cat_id == 'global_config') {
      Ext.getCmp('category_settings_css_prefix').hide(true);
      Ext.getCmp('category_settings_add_important').hide(true);
    }
    jQuery.post(Drupal.settings.basePath + 'style_manager/category_load', {cat: cat_id, token: Drupal.settings.style_manager.token}, function (data) {
      loadMask.hide();
      cat_id = data.id;
      Ext.getCmp('category_settings_id').setDisabled(true);
      Ext.getCmp('tab_import_export').setDisabled(false);
      Ext.getCmp('tab_style_form').setDisabled(false);
      Ext.getCmp('category_settings_id').setValue(data.id);
      Ext.getCmp('category_settings_title').setValue(data.title);
      Ext.getCmp('category_settings_css_prefix').setValue(data.css_prefix);
      Ext.getCmp('category_settings_add_important').setValue(data.add_important);
      if (data.from_code) {
        Ext.getCmp('category_settings_destroy').hide();
        Ext.getCmp('category_settings_reset').show();
      }
      else {
        Ext.getCmp('category_settings_destroy').show();
        Ext.getCmp('category_settings_reset').hide();
      }
    }, 'json');
  };

  Drupal.style_manager.settings_win_form = Ext.create('Ext.form.Panel', {
    frame: true,
    bodyPadding: 5,
    width: 1050,
    height: 440,
    fieldDefaults: {
      labelAlign: 'right',
      labelWidth: 100,
      msgTarget: 'qtip'
    },
    items: [
      {
        xtype: 'fieldset',
        defaultType: 'textfield',
        layout: 'anchor',
        padding: 10,
        title: 'Category settings',
        defaults: {
          anchor: '50%',
          hideEmptyLabel: false
        },
        items: [
          {
            name: 'id',
            fieldLabel: 'Id',
            id: 'category_settings_id',
            allowBlank: false,
            disabled: (cat_id) ? true : false,
            maskRe: /[A-Za-z0-9_]/,
            regex: /^[A-Za-z0-9_]*$/i
          },
          {
            name: 'title',
            fieldLabel: 'Title',
            id: 'category_settings_title',
            allowBlank: false
          },
          {
            name: 'css_prefix',
            fieldLabel: 'CSS prefix',
            id: 'category_settings_css_prefix',
            allowBlank: true
          },
          {
            checked: false,
            boxLabel: 'Add !important to all properties.',
            name: 'category_settings_add_important',
            xtype: 'checkbox',
            id: 'category_settings_add_important'
          }
        ]
      }
    ],

    buttons: [
      {
        text: 'Delete',
        id: 'category_settings_destroy',
        hidden: true,
        handler: function () {
          Ext.MessageBox.confirm('Confirm', 'Are you sure?', function (btn) {
            if (btn == 'no') {
              return;
            }
            var data = {
              cat_id: Ext.getCmp('category_settings_id').getValue(),
              token: Drupal.settings.style_manager.token
            };
            loadMask.show();
            Drupal.style_manager.styleStore.removeAll();
            jQuery.post(Drupal.settings.basePath + 'style_manager/category_destroy', data, function (data) {
              Drupal.style_manager.filter_category_tree_load();
              Drupal.style_manager.upd_page_styles('global_config');
              loadMask.hide();
              Drupal.style_manager.settings_win.destroy();
            }, 'json');
          });
        }
      },
      {
        text: 'Reset',
        hidden: true,
        id: 'category_settings_reset',
        handler: function () {
          Ext.MessageBox.confirm('Confirm', 'Are you sure?', function (btn) {
            if (btn == 'no') {
              return;
            }
            var data = {
              cat_id: Ext.getCmp('category_settings_id').getValue(),
              token: Drupal.settings.style_manager.token
            };
            loadMask.show();
            jQuery.post(Drupal.settings.basePath + 'style_manager/category_reset', data, function (data) {
              Drupal.style_manager.filter_category_tree_load(data.id);
              loadMask.hide();
              Drupal.style_manager.upd_page_styles('global_config')
              updInfo(data.id);
            }, 'json');
          });
        }
      },
      {
        text: 'Save',
        id: 'category_settings_save',
        handler: function () {
          if (this.up('form').getForm().isValid()) {
            var data = {
              cat_id: Ext.getCmp('category_settings_id').getValue(),
              cat_title: Ext.getCmp('category_settings_title').getValue(),
              css_prefix: Ext.getCmp('category_settings_css_prefix').getValue(),
              add_important: Ext.getCmp('category_settings_add_important').getValue(),
              old_cat_id: cat_id ? cat_id : '',
              token: Drupal.settings.style_manager.token
            };
            loadMask.show();
            jQuery.post(Drupal.settings.basePath + 'style_manager/category_save', data, function (data) {
              Drupal.style_manager.filter_category_tree_load(data.id);
              Drupal.style_manager.settings_win.destroy();
              Drupal.style_manager.upd_page_styles('global_config');
              style_manager_settings(data.id);
              Drupal.style_manager.filter_category_tree_load();
            }, 'json');
          }
        }
      },
      {
        text: 'Cancel',
        id: 'category_settings_cancel',
        handler: function () {
          Drupal.style_manager.settings_win.destroy();
        }
      }
    ]
  });

  Drupal.style_manager.export_import_form = Ext.create('Ext.form.Panel', {
    frame: true,
    id: 'export_import_form',
    bodyPadding: 5,
    width: 1050,
    height: 440,
    fieldDefaults: {
      labelAlign: 'top',
      labelWidth: 100
    },
    items: [
      {
        xtype: 'fieldset',
        defaultType: 'textfield',
        layout: 'anchor',
        padding: 10,
        title: 'Import category',
        items: [
          {
            xtype: 'filefield',
            fieldLabel: 'Select file',
            width: 350,
            hideLabel: false,
            allowBlank: false,
            name: 'settings'
          },
          {
            text: 'Import',
            xtype: 'button',
            minWidth: 100,
            margin: '5px 0 0 0',
            scale: 'medium',
            id: 'category_settings_import',
            handler: function () {
              var form = Ext.getCmp('export_import_form').getForm();
              if (form.isValid()) {
                var category_id = Ext.getCmp('category_settings_id').getValue();
                form.submit({
                  url: Drupal.settings.basePath + 'style_manager/category_import/' + category_id + '?token=' + Drupal.settings.style_manager.token,
                  waitMsg: 'Importing...',
                  success: function (fp, o) {
                    Ext.MessageBox.alert('Done', o.result.message);
                    Drupal.style_manager.filter_category_tree_load(o.result.id);
                    Drupal.style_manager.upd_page_styles();
                    updInfo(o.result.id);
                  },
                  failure: function (fp, o) {
                    Ext.MessageBox.alert('Error', o.result.message);
                  }
                });
              }
            }
          }
        ]
      },
      {
        xtype: 'fieldset',
        defaultType: 'textfield',
        layout: 'anchor',
        padding: 10,
        title: 'Export category',
        items: [
          {
            xtype: 'button',
            minWidth: 100,
            scale: 'medium',
            text: 'Export',
            id: 'category_settings_export',
            handler: function () {
              var category_id = Ext.getCmp('category_settings_id').getValue();
              window.open(Drupal.settings.basePath + 'style_manager/category_export/' + category_id + '/style_manager-' + category_id + '.settings');
            }
          }
        ]
      }
    ]
  });

  var groupcollapseUpd = function () {
    if (Drupal.style_manager.style_manager_settings_open_groups != false) {
      return false;
    }
    Drupal.style_manager.style_manager_settings_open_groups = {};
    jQuery('.x-grid-group-hd').not('.x-grid-group-hd-collapsed').find('.x-grid-group-title').each(function () {
      Drupal.style_manager.style_manager_settings_open_groups[jQuery(this).html()] = true
    });
  };
  Drupal.style_manager.settings_win_store = Ext.create('Ext.data.Store', {
    autoLoad: false,
    autoSync: true,
    model: 'settings_model',
    groupField: 'group',
    proxy: {
      type: 'ajax',
      api: {
        read: Drupal.settings.basePath + 'style_manager/category_form_item/load/' + cat_id,
        create: Drupal.settings.basePath + 'style_manager/category_form_item/save/' + cat_id + '/' + Drupal.style_manager.selected_cat,
        update: Drupal.settings.basePath + 'style_manager/category_form_item/save/' + cat_id,
        destroy: Drupal.settings.basePath + 'style_manager/category_form_item/destroy/' + cat_id
      },
      reader: {
        type: 'json',
        successProperty: 'success',
        root: 'data',
        messageProperty: 'message'
      },
      writer: {
        type: 'json',
        writeAllFields: true,
        root: 'data'
      },
      extraParams: {
        token: Drupal.settings.style_manager.token
      }
    },
    listeners: {
      write: function () {
        Drupal.style_manager.settings_win_store.load();
      },
      update: function () {
        Drupal.style_manager.settings_win_store.load();
      },
      beforeload: function () {
        groupcollapseUpd();
      },
      load: function () {
        var open_groups = Drupal.style_manager.style_manager_settings_open_groups;
        Drupal.style_manager.style_manager_settings_open_groups = false;
        jQuery('.x-grid-group-title').each(function () {
          if (open_groups[jQuery(this).html()] == true) {
            jQuery(this).click();
          }
        });
      }
    },
    sorters: [
      {
        property: 'weight',
        direction: 'ASC'
      },
      {
        property: 'title',
        direction: 'ASC'
      },
      {
        property: 'type',
        direction: 'ASC'
      },
      {
        property: 'selector',
        direction: 'ASC'
      }
    ]
  });

  var groupingFeature = Ext.create('Ext.grid.feature.Grouping', {
    groupHeaderTpl: '{name}',
    startCollapsed: true
  });

  var rowEditing = Ext.create('Ext.grid.plugin.RowEditing', {
    clicksToMoveEditor: 1,
    listeners: {
      beforeedit: function (editor, e, eOpts) {
        if (editor.record.data.locked) {
          Ext.getCmp('grid_form_type').setDisabled(true);
        }
        else {
          Ext.getCmp('grid_form_type').setDisabled(false);
        }
      }
    }
  });

  var typeUpd = function (el) {
    var selector_field = Ext.getCmp('grid_form_selector');
    var is_global = (el.value == 'style_manager_global_color' || el.value == 'style_manager_global_font' || el.value == 'style_manager_global_numeric');
    if (el.value == 'css_code' || is_global) {
      selector_field.setDisabled(true);
    }
    else {
      selector_field.setDisabled(false);
    }
  };

  Drupal.style_manager.settings_win_grid = Ext.create('Ext.grid.Panel', {
    plugins: [rowEditing],
    features: [groupingFeature],
    width: 1050,
    height: 440,
    store: Drupal.style_manager.settings_win_store,
    columns: [
      {
        text: 'ID',
        width: 120,
        hidden: true,
        sortable: false,
        dataIndex: 'id'
      },
      {
        text: 'Group',
        width: 120,
        dataIndex: 'group',
        sortable: false,
        field: {
          selectOnFocus: true,
          allowBlank: false,
          xtype: 'textfield'
        }
      },
      {
        header: 'Title',
        width: 120,
        id: 'grid_form_title',
        dataIndex: 'title',
        sortable: false,
        field: {
          selectOnFocus: true,
          xtype: 'textfield',
          allowBlank: cat_id != 'global_config'
        }
      },
      {
        text: 'Type',
        width: 120,
        flex: cat_id == 'global_config',
        dataIndex: 'type',
        sortable: false,
        renderer: function (val) {
          var title = val;
          var widgets = (cat_id == 'global_config') ? Drupal.settings.style_manager.global_widgets : Drupal.settings.style_manager.widgets;
          jQuery.each(widgets, function (index, value) {
            if (value.value == val) {
              title = value.name;
            }
          });
          return title;
        },
        field: {
          allowBlank: false,
          xtype: 'combo',
          id: 'grid_form_type',
          mode: 'local',
          triggerAction: 'all',
          editable: false,
          displayField: 'name',
          valueField: 'value',
          sortable: false,
          store: {
            xtype: 'store',
            fields: ['value', 'name', 'description'],
            data: (cat_id == 'global_config') ? Drupal.settings.style_manager.global_widgets : Drupal.settings.style_manager.widgets
          },
          listConfig: {
            getInnerTpl: function () {
              return '<div  data-qtip="{description}" style="white-space: nowrap;">{name}</div>';
            }
          },
          listeners: {
            afterrender: typeUpd,
            change: typeUpd
          }
        }
      },
      {
        text: 'CSS Selector',
        flex: 1,
        dataIndex: 'selector',
        hidden: cat_id == 'global_config',
        sortable: false,
        field: {
          selectOnFocus: true,
          disabled: cat_id == 'global_config',
          allowBlank: true,
          id: 'grid_form_selector',
          xtype: 'cssinspector'
        }
      },
      {
        text: 'Weight',
        width: 60,
        dataIndex: 'weight',
        sortable: false,
        field: {
          selectOnFocus: false,
          disabled: false,
          allowBlank: true,
          id: 'grid_form_weight',
          step: 50,
          xtype: 'numberfield'
        }
      }
    ],
    dockedItems: [
      {
        xtype: 'toolbar',
        items: [
          {
            text: 'Add',
            iconCls: 'add_icon',
            handler: function () {
              var fields = {};
              if (Drupal.style_manager.settings_win_grid.getView().getSelectionModel().getSelection()[0]) {
                var selected = Drupal.style_manager.settings_win_grid.getView().getSelectionModel().getSelection()[0]['data'];
                fields.group = selected.group;
                fields.selector = selected.selector;
              }
              Drupal.style_manager.settings_win_store.insert(0, new settings_model(fields));
              rowEditing.startEdit(0, 0);
            }
          },
          '-',
          {
            text: 'Delete',
            iconCls: 'del_icon',
            id: 'style_manager_btn_styles_table_delete',
            handler: function () {
              Ext.MessageBox.confirm('Confirm', 'Are you sure?', function (btn) {
                if (btn == 'no') {
                  return;
                }
                var selection = Drupal.style_manager.settings_win_grid.getView().getSelectionModel().getSelection()[0];
                if (selection) {
                  Drupal.style_manager.settings_win_store.remove(selection);
                }
              });
            }
          },
          '->',
          {
            iconCls: 'refresh_icon',
            id: 'style_manager_btn_styles_table_update',
            handler: function () {
              Drupal.style_manager.settings_win_store.load();
            }
          }
        ]
      }
    ],
    listeners: {
      selectionchange: function (selModel, selections) {
        if (selections[0] && selections[0].data.locked) {
          Ext.getCmp('style_manager_btn_styles_table_delete').setDisabled(true);
        }
        else {
          Ext.getCmp('style_manager_btn_styles_table_delete').setDisabled(false);
        }
      },
      groupclick: function () {
        Drupal.style_manager.style_manager_settings_open_groups = false;
        groupcollapseUpd();
      }
    }
  });

  Drupal.style_manager.settings_win = Ext.create('Ext.window.Window', {
    title: (cat_id) ? 'Category settings' : 'New category',
    id: 'settings_window',
    iconCls: (cat_id) ? 'gear_icon' : 'new_category_icon',
    hidden: false,
    collapsible: false,
    stateful: false,
    width: 1065,
    height: 500,
    resizable: false,
    listeners: {
      beforedestroy: function () {
        if (Drupal.style_manager.selected_cat_arr && Drupal.style_manager.selected_cat_arr[1]) {
          Drupal.style_manager.styleStore.load({url: Drupal.style_manager.styleStoreProxy.url + '?cat=' + Drupal.style_manager.selected_cat});
        }
        Drupal.style_manager.upd_page_styles();
      }
    },
    items: [
      {
        activeTab: act_tab,
        xtype: 'tabpanel',
        defaults: {
          bodyPadding: 0
        },
        items: [
          {
            title: 'Settings',
            items: Drupal.style_manager.settings_win_form
          },
          {
            title: 'Style form',
            id: 'tab_style_form',
            disabled: !(cat_id),
            items: Drupal.style_manager.settings_win_grid
          },
          {
            title: 'Import / export',
            id: 'tab_import_export',
            disabled: !(cat_id),
            items: Drupal.style_manager.export_import_form
          }
        ]
      }
    ]
  });
  var parentWinPosition = Drupal.style_manager.win.getPosition();
  Drupal.style_manager.settings_win.setPosition(parentWinPosition[0], parentWinPosition[1]);
  Drupal.style_manager.settings_win.show();
  var loadMask = new Ext.LoadMask(Ext.getCmp('settings_window'), {msg: "Please wait...", shadow: true, shrinkWrap: true});
  if (cat_id) {
    updInfo(cat_id);
  }
}
