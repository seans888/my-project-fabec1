Drupal.style_manager.form_listeners = {
//  widget: {
//    element: {
//      afterrender: function(id) {},
//      change: function(id) {}
//    }
//  },

  css_code: {
    color_overriding: {
      change: function (id, value) {
        if (Ext.getCmp(id + '_images_overriding')) {
          Ext.getCmp(id + '_images_overriding').setDisabled(!value);
        }
        if (Ext.getCmp(id + '_image_options')) {
          Ext.getCmp(id + '_image_options').setDisabled(!value);
        }
        for (i = 1; i < 30; i++) {
          var tab_id = id + '--color' + i + '--tab';
          if (Ext.getCmp(tab_id)) {
            Ext.getCmp(tab_id).setDisabled(!value);
          }
          else {
            return;
          }
        }
      }
    }
  },

  background: {
    image_url: {
      afterrender: function (id, output_selectors) {
        var combo = Ext.getCmp(id + '--image--url');
        var bgData = {};
        jQuery(output_selectors).each(function () {
          var bg = jQuery(this).css('background-image');
          if (bg && bg != 'none') {
            var imgUrl = bg.replace(/url\(|\)/ig, '').replace(/\"/gi, '').replace(/\'/gi, '');
            if (imgUrl.indexOf('data:') == -1 && imgUrl.indexOf('gradient') == -1) {
              bgData[imgUrl] = imgUrl;
            }
          }
        });
        jQuery.each(bgData, function (index, value) {
          combo.store.add({
            'value': value,
            'name': value,
            'name_html': value,
            'description': value
          });
        });
        if (combo.getValue() == '') {
          for (var key in bgData) {
            break;
          }
          combo.setValue(key);
        }
      }
    }
  }
};
