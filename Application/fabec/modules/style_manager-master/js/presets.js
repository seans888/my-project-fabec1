Ext.onReady(function () {
  Drupal.style_manager.catStore = Ext.create('Ext.data.TreeStore', {
    proxy: {
      type: 'ajax',
      url: Drupal.settings.basePath + 'style_manager/get_cat_list.json',
      extraParams: {
        token: Drupal.settings.style_manager.token
      }
    }
  });

  Drupal.style_manager.catTree = Ext.create('Ext.tree.Panel', {
    store: Drupal.style_manager.catStore,
    rootVisible: false,
    useArrows: true,
    frame: false,
    singleExpand: true,
    width: 170,
    anchor: '100%',
    tbar: [
      {
        text: 'New category',
        iconCls: 'new_category_icon',
        id: 'style_manager_btn_category_new',
        disabled: false,
        handler: function () {
          style_manager_settings('');
        }
      }
    ],
    listeners: {
      selectionchange: function (selModel, selections) {
        Ext.getCmp('style_manager_cat_table').enable();
        Ext.getCmp('style_manager_el_form').enable();
        if (!selections[0]) {
          Drupal.style_manager.selected_cat = false;
          Drupal.style_manager.selected_cat_arr = [];
          Ext.getCmp('style_manager_btn_category_settings').setDisabled(true);
          Ext.getCmp('style_manager_el_form').disable();
          Ext.getCmp('style_manager_cat_table').disable();
          return false;
        }
        if (selections[0].data.depth == 1) {
          Drupal.style_manager.selected_cat = selections[0].data.id;
          Drupal.style_manager.selected_cat_arr = [selections[0].data.id];
          Drupal.style_manager.previous_selected_preset = '';
          Drupal.style_manager.previous_proxy = '';
          Drupal.style_manager.styleStore.removeAll();
        }
        else {
          Drupal.style_manager.selected_cat_data = selections[0];
          Drupal.style_manager.selected_cat = selections[0].data.id;
          Drupal.style_manager.selected_cat_arr = selections[0].data.id.split('--');
          if (Drupal.style_manager.selected_cat_arr[1]
              && Drupal.style_manager.selected_cat_arr[1] != 'disable'
              && (!Drupal.style_manager.previous_proxy
                 || Drupal.style_manager.previous_proxy != Drupal.style_manager.selected_cat_arr[0])
          ) {
            Drupal.style_manager.previous_proxy = Drupal.style_manager.selected_cat_arr[0];
            Drupal.style_manager.styleStore.load({url: Drupal.style_manager.styleStoreProxy.url + '?cat=' + selections[0].data.id});
          }
          else {
            Drupal.style_manager.styleGrid.getSelectionModel().deselectAll();
          }
          if (Drupal.style_manager.selected_cat_arr[1]) {
            Drupal.style_manager.upd_page_styles();
            if (Drupal.style_manager.selected_cat_arr[0] != Drupal.style_manager.previous_selected_cat) {
              Drupal.style_manager.upd_page_styles(Drupal.style_manager.previous_selected_cat);
            }
            if (Drupal.style_manager.selected_cat_arr[1] == 'disable') {
              Ext.getCmp('style_manager_el_form').disable();
              Ext.getCmp('style_manager_cat_table').disable();
            }
          }
          Drupal.style_manager.previous_selected_preset = Drupal.style_manager.selected_cat_arr[1];
          Drupal.style_manager.previous_selected_cat = Drupal.style_manager.selected_cat_arr[0];
        }
      }
    }
  });
});
