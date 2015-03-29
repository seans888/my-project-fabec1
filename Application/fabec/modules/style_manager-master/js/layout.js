Ext.require([
  'Ext.tip.*',
  'Ext.form.*',
  'Ext.window.Window',
  //'Ext.state.*',
  'Ext.window.MessageBox',
  'Ext.tip.QuickTipManager',
  'Ext.tip.*',
  'Ext.tree.*',
  'Ext.data.*'
]);
Ext.scopeResetCSS = true;

Ext.onReady(function () {
  if (Ext.getCmp('style_manager_el_form') || (Drupal.overlay && Drupal.overlay.isOpen)) {
    return false;
  }
  Ext.tip.QuickTipManager.init();
  Drupal.style_manager.add_style_settings_btn();
  //Ext.state.Manager.setProvider(Ext.create('Ext.state.CookieProvider'));
  var border = Ext.create('Ext.panel.Panel', {
    layout: {
      type: 'border'
    },
    stateId: 'stateBorder',
    stateful: true,
    border: false,
    items: [
      {
        region: 'west',
        id: 'west-panel',
        title: 'Category and presets',
        iconCls: 'categories_icon',
        split: true,
        border: false,
        width: 175,
        minWidth: 175,
        maxWidth: 175,
        layout: 'fit',
        items: Drupal.style_manager.catTree
      },
      {
        region: 'center',
        title: 'Groups',
        iconCls: 'category_icon',
        id: 'style_manager_cat_table',
        border: false,
        width: 175,
        minWidth: 175,
        maxWidth: 175,
        layout: 'fit',
        items: Drupal.style_manager.styleGrid
      },
      {
        split: true,
        width: 695,
        height: 440,
        title: 'Settings',
        iconCls: 'category-item_icon',
        region: 'east',
        border: false,
        //layout: 'fit',
        autoScroll: false,
        id: 'style_manager_el_form',
        items: [
          {
            //layout: 'fit',
            height: 440,
            frame: true
          }
        ]
      }
    ]
  });

  Drupal.style_manager.output_css_win = Ext.create('Ext.window.Window', {
    title: 'Output CSS',
    stateful: true,
    stateId: 'stateOutputCssWindow',
    height: 500,
    width: 400,
    layout: 'fit',
    plain: true,
    closeAction: 'hide',
    items: [
      {
        autoScroll: true,
        html: '<pre id="style_manager_output_css"ospace; font-size: 14px; line-height: 120%;"></pre>'
      }
    ],
    listeners: {
      afterrender: function () {
        Drupal.style_manager.upd_page_output_css_textarea();
      }
    }
  });

  var style_manager_win_width = 1065;
  Drupal.style_manager.win = Ext.create('Ext.window.Window', {
    title: Drupal.settings.style_manager.title,
    iconCls: 'status',
    collapsible: true,
    stateful: true,
    resizable: false,
    stateId: 'stateWindow',
    height: 500,
    width: style_manager_win_width,
    minWidth: style_manager_win_width,
    maxWidth: style_manager_win_width,
    minHeight: 400,
    layout: 'fit',
    plain: true,
    closeAction: 'hide',
    items: border,
    listeners: {
      afterrender: function () {
        jQuery('link[href*="style_manager_cache/css"]').remove();
        Drupal.style_manager.upd_page_styles();
        Ext.getCmp('style_manager_el_form').disable();
        Ext.getCmp('style_manager_cat_table').disable();
      },
      beforehide: function () {
        if (!$('#style_settings_btn').is(':hover')) {
          jQuery('#style_settings_btn').stop().animate({width: "10px"}, {duration: 100});
          Drupal.style_manager.win_hide(false);
        }
      }
    },
    tools: [
      {
        handler: function () {
          if (Drupal.style_manager.output_css_win.isHidden()) {
            var parentWinPosition = Drupal.style_manager.win.getPosition();
            Drupal.style_manager.output_css_win.showAt(parentWinPosition[0] - 400, parentWinPosition[1]);
          }
          else {
            Drupal.style_manager.output_css_win.hide();
          }
        },
        is: 'style_manager_gear_btn',
        type: 'gear'
      }
    ]
  }).setPosition(($(document).width() - 1120));
  Drupal.style_manager.loadMask = new Ext.LoadMask(Ext.getCmp('style_manager_el_form'), {msg: "Please wait...", shadow: true, shrinkWrap: true});
  jQuery('body').removeClass('x-body');
  Drupal.style_manager.loop();
});
