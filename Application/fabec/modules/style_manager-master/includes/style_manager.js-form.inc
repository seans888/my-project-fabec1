<?php

/**
 * @file
 * Settings for ext-js forms.
 */

/**
 * Type Reset.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_reset($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['type'] = isset($settings['type']) ? $settings['type'] : 'all';

  $listeners = " function() {
                      var type = Ext.getCmp('{$id}--type');
                      if (type.value == 'custom') {
                        Ext.getCmp('{$variables['data']['id']}--checkboxgroup').show();
                      }
                      else {
                        Ext.getCmp('{$variables['data']['id']}--checkboxgroup').hide();
                      }
                 }
  ";
  $type_options = array(
    array('all', 'All', 'All'),
    array('custom', 'Custom', 'Custom'),
    array('none', 'None', 'None'),
  );
  $type_settings = array(
    'id' => $id . '--type',
    'val' => $settings['type'],
    'options' => $type_options,
    'listeners' => $listeners,
    'afterrender' => $listeners,
    'width' => 205,
  );
  $type_field = _style_manager_field_combo($type_settings);

  $checkboxgroup = array(
    'xtype' => 'checkboxgroup',
    'id' => $variables['data']['id'] . '--checkboxgroup',
    'fieldLabel' => 'Single Column',
    'columns' => 2,
    'width' => 300,
    'padding' => '0 0 0 20px',
    'items' => array(
      array(
        'boxLabel' => t('Background'),
        'name' => $id . '--custom--background',
        'checked' => !empty($settings['custom']['background']),
      ),
      array(
        'boxLabel' => t('Border'),
        'name' => $id . '--custom--border',
        'checked' => !empty($settings['custom']['border']),
      ),
      array(
        'boxLabel' => t('Font'),
        'name' => $id . '--custom--font',
        'checked' => !empty($settings['custom']['font']),
      ),
      array(
        'boxLabel' => t('List style'),
        'name' => $id . '--custom--list_style',
        'checked' => !empty($settings['custom']['list_style']),
      ),
      array(
        'boxLabel' => t('Margin'),
        'name' => $id . '--custom--margin',
        'checked' => !empty($settings['custom']['margin']),
      ),
      array(
        'boxLabel' => t('Padding'),
        'name' => $id . '--custom--padding',
        'checked' => !empty($settings['custom']['padding']),
      ),
      array(
        'boxLabel' => t('Position'),
        'name' => $id . '--custom--position',
        'checked' => !empty($settings['custom']['position']),
      ),
    ),
  );
  $items = array();
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Type'),
    'items' => array($type_field, $checkboxgroup),
  ));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}

/**
 * Type List style.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_list_style($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['style'] = isset($settings['style']) ? $settings['style'] : 'solid';

  $type_options = array(
    '-',
    'none',
    'circle',
    'disc',
    'square',
    'armenian',
    'decimal',
    'decimal-leading-zero',
    'georgian',
    'lower-alpha',
    'lower-greek',
    'lower-latin',
    'lower-roman',
    'upper-alpha',
    'upper-latin',
    'upper-roman',
  );
  $type_field = _style_manager_field_combo(array(
    'id' => $id . '--type',
    'width' => 205,
    'val' => $settings['type'],
    'options' => $type_options,
  ));
  $position_options = array('-', 'inside', 'outside');
  $position_field = _style_manager_field_combo(array(
    'id' => $id . '--position',
    'width' => 205,
    'val' => $settings['position'],
    'options' => $position_options,
  ));
  $image_options = array('-', 'none');
  $image_field = _style_manager_field_combo(array(
    'id' => $id . '--image',
    'width' => 280,
    'val' => $settings['image'],
    'options' => $image_options,
    'title' => t('Image'),
    'editable' => TRUE,
  ));

  $items = array();
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Type'),
    'items' => array($type_field, $image_field),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Position'),
    'items' => array($position_field),
  ));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}


/**
 * Type Font.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_font($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['style'] = isset($settings['style']) ? $settings['style'] : 'solid';
  $tabs = array();

  $settings['font_family'] = isset($settings['font_family']) ? $settings['font_family'] : '-';
  $settings['font_weight'] = isset($settings['font_weight']) ? $settings['font_weight'] : '-';
  $settings['font_style'] = isset($settings['font_style']) ? $settings['font_style'] : '-';
  $settings['text_align'] = isset($settings['text_align']) ? $settings['text_align'] : '-';
  $settings['text_transform'] = isset($settings['text_transform']) ? $settings['text_transform'] : '-';
  $settings['text_decoration'] = isset($settings['text_decoration']) ? $settings['text_decoration'] : '-';
  $settings['line_height'] = isset($settings['line_height']) ? $settings['line_height'] : '';
  $settings['font_size'] = isset($settings['font_size']) ? $settings['font_size'] : '';
  $settings['shadow']['offset_x'] = isset($settings['shadow']['offset_x']) ? $settings['shadow']['offset_x'] : '';
  $settings['shadow']['offset_y'] = isset($settings['shadow']['offset_y']) ? $settings['shadow']['offset_y'] : '';
  $settings['shadow']['blur'] = isset($settings['shadow']['blur']) ? $settings['shadow']['blur'] : '';

  $globals_font = style_manager_get_globals_var('global_font');
  $font_family_options[] = array('-', '-', '-');
  if ($globals_font) {
    foreach ($globals_font as $key => $var) {
      $font_family_options[] = array(
        $key,
        strip_tags($var['title']),
        $var['title'],
      );
    }
  }
  $font_family_field = _style_manager_field_combo(array(
    'id' => $id . '--font_family',
    'width' => 205,
    'val' => $settings['font_family'],
    'options' => $font_family_options,
  ));
  $font_weight_options = array('-', 'normal', 'bold');
  $font_weight_field = _style_manager_field_combo(array(
    'id' => $id . '--font_weight',
    'width' => 280,
    'val' => $settings['font_weight'],
    'options' => $font_weight_options,
    'title' => t('Weight'),
  ));
  $font_style_options = array('-', 'normal', 'italic', 'oblique');
  $font_style_field = _style_manager_field_combo(array(
    'id' => $id . '--font_style',
    'width' => 280,
    'val' => $settings['font_style'],
    'options' => $font_style_options,
    'title' => t('Style'),
  ));
  $text_align_options = array(
    '-',
    'center',
    'justify',
    'left',
    'right',
    'start',
    'end',
  );
  $text_align_field = _style_manager_field_combo(array(
    'id' => $id . '--text_align',
    'width' => 205,
    'val' => $settings['text_align'],
    'options' => $text_align_options,
  ));
  $text_transform_options = array(
    '-',
    'none',
    'capitalize',
    'lowercase',
    'uppercase',
  );
  $text_transform_field = _style_manager_field_combo(array(
    'id' => $id . '--text_transform',
    'width' => 280,
    'val' => $settings['text_transform'],
    'options' => $text_transform_options,
    'title' => t('Transform'),
  ));
  $text_decoration_options = array(
    '-',
    'none',
    'underline',
    'blink',
    'line-through',
    'overline',
  );
  $text_decoration_field = _style_manager_field_combo(array(
    'id' => $id . '--text_decoration',
    'width' => 280,
    'val' => $settings['text_decoration'],
    'options' => $text_decoration_options,
    'title' => t('Decoration'),
  ));
  $line_height_field = _style_manager_jsform_advanced_numeric(array(
    'id' => $id . '--line_height',
    'value' => $settings['line_height'],
    'min' => 0,
  ));
  $font_size_field = _style_manager_jsform_advanced_numeric(array(
    'id' => $id . '--font_size',
    'value' => $settings['font_size'],
    'min' => 0,
  ));

  $items = array();
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Font'),
    'items' => array($font_family_field, $font_weight_field),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Align'),
    'items' => array($text_align_field, $text_decoration_field),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Font-size'),
    'items' => array(
      $font_size_field,
      array('xtype' => 'displayfield', 'width' => 75),
      $font_style_field,
    ),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Line-height'),
    'items' => array(
      $line_height_field,
      array('xtype' => 'displayfield', 'width' => 75),
      $text_transform_field,
    ),
  ));

  $tabs[] = array(
    'title' => t('Font options'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => _style_manager_jsform_fieldset(array(
      'title' => '',
      'collapsible' => FALSE,
      'items' => $items,
    )),
  );
  $color_options = array(
    'settings' => empty($settings['color']) ? FALSE : $settings['color'],
    'id' => $id . '--color',
    'fields_id' => $id . '--color--',
    'hide_fieldset' => FALSE,
  );
  $tabs[] = array(
    'title' => t('Font color'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => style_manager_color(array('data' => $color_options)),
  );

  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--shadow--offset_x',
      'value' => $settings['shadow']['offset_x'],
    )),
    array(
      'xtype' => 'displayfield',
      'value' => ' (Y):',
      'margin' => '0 3px 0 3px',
    ),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--shadow--offset_y',
      'value' => $settings['shadow']['offset_y'],
    )),
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => t('Blur'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 40,
      'margin' => '0 0 0 5px',
    ),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--shadow--blur',
      'value' => $settings['shadow']['blur'],
      'min' => 0,
    )),
  );

  $shadow_items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Offset (X)'),
    'items' => $fields,
    'margin' => '0 0 10px 0',
  ));

  $color_options = array(
    'settings' => empty($settings['shadow']['color']) ? FALSE : $settings['shadow']['color'],
    'id' => $id . '--shadow--color',
    'fields_id' => $id . '--shadow--color--',
    'hide_fieldset' => TRUE,
  );
  $shadow_items[] = style_manager_color(array('data' => $color_options));

  $tabs[] = array(
    'title' => t('Font shadow'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => _style_manager_jsform_fieldset(array(
      'title' => '',
      'collapsible' => FALSE,
      'items' => $shadow_items,
    )),
  );

  $tabpanel = _style_manager_jsform_tabpanel(array('items' => $tabs));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $tabpanel,
  ));
}

/**
 * Type Gradient.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_gradient($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';

  // Color.
  $color_options = array(
    'settings' => empty($settings['color']) ? FALSE : $settings['color'],
    'id' => $id,
    'fields_id' => $id . '--color--',
    'hide_fieldset' => TRUE,
  );
  $gradient_color_items = style_manager_color(array('data' => $color_options));

  // Gradient presets.
  $output_selectors = implode(',', $variables['data']['output_selectors']);
  $replace_from = array(
    ':link',
    ':hover',
    ':visited',
    ':active',
  );
  $output_selectors = str_replace($replace_from, '', $output_selectors);
  $preset_val = isset($settings['preset']) ? $settings['preset'] : '-';
  $preset_field_afterrender = "function() {
                      if (!Ext.getCmp('{$id}--preset').is_init) {
                        Ext.getCmp('{$id}--preset').is_init = true;
                        var combo = Ext.getCmp('{$id}--preset');
                        jQuery.each(Drupal.style_manager.gradient_presets, function (index, value) {
                          combo.store.add({
                            'value': index,
                            'name': value.title,
                            'name_html': value.title,
                            'description': value.title
                          });
                        });
                        if (combo.getValue() == '-' || combo.getValue() == 'custom') {
                          setTimeout(function(){Ext.getCmp('{$id}--position').disable();}, 200);
                        }
                        setTimeout(function(){Ext.getCmp('{$id}--preset').setValue('{$preset_val}')}, 200);
                        if (combo.getValue() == 'custom' && Ext.getCmp('{$id}--css_code').getValue() == '') {
                          jQuery('$output_selectors').each(function () {
                            var bg = jQuery(this).css('background-image');
                            if (bg.indexOf('gradient') != -1) {
                              setTimeout(\"Ext.getCmp('{$id}--css_code').setValue('background: \"+ bg +\";');\", 300);
                            }
                            return ;
                          });
                        }
                      }
  }";
  $preset_field_listeners = "function() {
                      if (Ext.getCmp('{$id}--preset') && Ext.getCmp('{$id}--position')) {
                        var preset_val = Ext.getCmp('{$id}--preset').getValue();
                        if (preset_val == '-' || preset_val == 'custom') {
                          Ext.getCmp('{$id}--position').disable();
                        }
                        else if (Drupal.style_manager.gradient_presets[preset_val]) {
                          Ext.getCmp('{$id}--position').enable();
                          var position_val = Ext.getCmp('{$id}--position').getValue();
                          var css_code = Drupal.style_manager.gradient_presets[preset_val].css_code;
                          if (Drupal.style_manager.gradient_presets_replacement[position_val]) {
                            jQuery.each(Drupal.style_manager.gradient_presets_replacement[position_val], function (index, value) {
                              css_code = css_code.split(value.from).join(value.to);
                            });
                          }
                          Drupal.style_manager.gradient_preset_setvalue_act = true;
                          setTimeout(function(){Drupal.style_manager.gradient_preset_setvalue_act = false;}, 800);
                          Ext.getCmp('{$id}--css_code').setValue(css_code);
                        }
                      }
  }";

  $position_options = array(
    array('horizontal', 'Horizontal  в†’', 'Horizontal  в†’'),
    array('vertical', 'Vertical  в†“', 'Vertical  в†“'),
    array('diagonal', 'Diagonal  в†', 'Diagonal  в†'),
    array('diagonal-bottom', 'Diagonal  в†—', 'Diagonal  в†—'),
    array('radial', 'Radial  в—‹', 'Radial  в—‹'),
  );
  $preset_options = array(
    array('-', '-', '-'),
    array('custom', 'Custom', 'Custom'),
  );
  $position_field = _style_manager_field_combo(array(
    'id' => $id . '--position',
    'val' => isset($settings['position']) ? $settings['position'] : '-',
    'options' => $position_options,
    'listeners' => $preset_field_listeners,
    'width' => 110,
  ));
  $preset_field = _style_manager_field_combo(array(
    'id' => $id . '--preset',
    'val' => $preset_val,
    'options' => $preset_options,
    'afterrender' => $preset_field_afterrender,
    'listeners' => $preset_field_listeners,
    'width' => 200,
  ));

  $presets_container = _style_manager_jsform_fieldcontainer(array(
    'title' => 'Preset',
    'margin' => '0 0 5px 0',
    'items' => array(
      $preset_field,
      array(
        'xtype' => 'displayfield',
        'fieldLabel' => 'Position',
        'labelAlign' => 'right',
        'hideLabel' => FALSE,
        'labelWidth' => 59,
        'margin' => '0 0px 0 5px',
      ),
      $position_field,
    ),
  ));

  // Color mode.
  $color_mode_field = _style_manager_field_combo(array(
    'id' => $id . '--color_mode',
    'val' => isset($settings['color_mode']) ? $settings['color_mode'] : '-',
    'options' => style_manager_color_overriding_options(),
    'width' => 390,
  ));

  $img_settings_container = _style_manager_jsform_fieldcontainer(array(
    'title' => 'Color mode',
    'items' => $color_mode_field,
    'margin' => '0 0 5px 0',
  ));

  array_unshift($gradient_color_items, $img_settings_container);
  array_unshift($gradient_color_items, $presets_container);
  $option_tab = _style_manager_jsform_fieldset(array(
    'title' => '',
    'collapsible' => FALSE,
    'items' => $gradient_color_items,
  ));

  $css_code_tab = array();
  $css_code = array(
    'xtype' => 'textarea',
    'listeners' => "<js_start> { change: function(){ if (!Drupal.style_manager.gradient_preset_setvalue_act) Ext.getCmp('{$id}--preset').setValue('custom'); }  } <js_end>",
    'id' => $id . '--css_code',
    'name' => $id . '--css_code',
    'value' => $settings['css_code'],
    'height' => 150,
    'margin' => '0 0 5px 0',
    'width' => 515,
  );
  $css_code_tab[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('CSS code'),
    'items' => $css_code,
    'margin' => '0',
  ));
  $css_code_tab[] = _style_manager_jsform_fieldcontainer(array(
    'items' => array(
      'xtype' => 'displayfield',
      'value' => 'To create your own gradient, you can use <a href="http://www.colorzilla.com/gradient-editor/" target="_blank">CSS Gradient Generator</a>.',
      'labelAlign' => 'right',
      'hideLabel' => TRUE,
      'margin' => '0 0 5px 0',
    ),
    'margin' => '0',
    'title' => '',
  ));

  $tabs = array();
  $tabs[] = array(
    'title' => t('Border options'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => array($option_tab),
  );
  $tabs[] = array(
    'title' => t('Edit css'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => $css_code_tab,
  );

  $tabpanel = _style_manager_jsform_tabpanel(array('items' => $tabs));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $tabpanel,
  ));
}

/**
 * Type Border.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_border($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['style'] = isset($settings['style']) ? $settings['style'] : 'solid';
  $settings['width'] = isset($settings['width']) ? $settings['width'] : '1px';

  $items = array();
  $listeners_fn = " function() {
                      if (Ext.getCmp('{$id}_different_color').getValue()) {
                        jQuery(['top', 'right', 'bottom', 'left']).each(function (key, val) {
                          if (Ext.getCmp('fieldset_{$id}_' + val)) {
                            if (Ext.getCmp('{$id}_' + val).getValue()){
                              Ext.getCmp('fieldset_{$id}_' + val).show();
                            }
                            else {
                              Ext.getCmp('fieldset_{$id}_' + val).hide();
                            }
                          }
                          Ext.getCmp('fieldset_{$id}_default').hide();
                        });
                      }
                      else {
                        jQuery(['top', 'right', 'bottom', 'left']).each(function (key, val) {
                          if (Ext.getCmp('fieldset_{$id}_' + val)) {
                            Ext.getCmp('fieldset_{$id}_' + val).hide();
                          }
                          Ext.getCmp('fieldset_{$id}_default').show();
                        });
                      }
                 }
  ";
  $listeners_fn = str_replace(array("\r", "\n"), array("", ""), $listeners_fn);
  $listeners = "<js_start> { change: {$listeners_fn}, afterrender: {$listeners_fn} } <js_end>";

  $border_arr = array(
    array(
      'xtype' => 'checkboxfield',
      'margin' => '0 0 0 2px',
      'boxLabel' => t('Top'),
      'checked' => !empty($settings['top']) ? TRUE : FALSE,
      'name' => $id . '--top',
      'id' => $id . '_top',
      'listeners' => $listeners,
    ),
    array(
      'xtype' => 'checkboxfield',
      'margin' => '0 0 0 10px',
      'boxLabel' => t('Right'),
      'checked' => !empty($settings['right']) ? TRUE : FALSE,
      'name' => $id . '--right',
      'id' => $id . '_right',
      'listeners' => $listeners,
    ),
    array(
      'xtype' => 'checkboxfield',
      'margin' => '0 0 0 10px',
      'boxLabel' => t('Bottom'),
      'checked' => !empty($settings['bottom']) ? TRUE : FALSE,
      'name' => $id . '--bottom',
      'id' => $id . '_bottom',
      'listeners' => $listeners,
    ),
    array(
      'xtype' => 'checkboxfield',
      'margin' => '0 0 5px 10px',
      'boxLabel' => t('Left'),
      'checked' => !empty($settings['left']) ? TRUE : FALSE,
      'name' => $id . '--left',
      'id' => $id . '_left',
      'listeners' => $listeners,
    ),
  );

  $width = _style_manager_jsform_advanced_numeric(array(
    'id' => $id . '--width',
    'title' => t('Width'),
    'value' => $settings['width'],
    'options' => array('-', 'thin', 'medium', 'thick'),
  ));
  $style_options = array(
    '-',
    'none',
    'solid',
    'hidden',
    'dotted',
    'dashed',
    'double',
    'groove',
    'ridge',
    'inset',
    'outset',
    'inherit',
  );
  $style_field = _style_manager_field_combo(array(
    'id' => $id . '--style',
    'width' => 205,
    'val' => $settings['style'],
    'options' => $style_options,
  ));

  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Border'),
    'items' => $border_arr,
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Style'),
    'items' => array($style_field, $width),
    'margin' => '0 0 5px 0',
  ));

  $different_color = array(
    'xtype' => 'checkboxfield',
    'margin' => '0 0 0px 5px',
    'boxLabel' => t('Different colors'),
    'checked' => !empty($settings['different_color']) ? TRUE : FALSE,
    'name' => $id . '--different_color',
    'id' => $id . '_different_color',
    'listeners' => $listeners,
  );
  $default_color_options = array(
    'settings' => empty($settings['color']['default']) ? FALSE : $settings['color']['default'],
    'color_field_title' => t('Color'),
    'id' => $id . '_default',
    'fields_id' => $id . '--color--default--',
    'hide_fieldset' => FALSE,
  );
  $l_color_options = array(
    'settings' => empty($settings['color']['left']) ? FALSE : $settings['color']['left'],
    'color_field_title' => t('Border left'),
    'id' => $id . '_left',
    'fields_id' => $id . '--color--left--',
    'default' => 'Border default',
    'hide_fieldset' => FALSE,
  );
  $r_color_options = array(
    'settings' => empty($settings['color']['right']) ? FALSE : $settings['color']['right'],
    'color_field_title' => t('Border right'),
    'id' => $id . '_right',
    'fields_id' => $id . '--color--right--',
    'default' => 'Border default',
    'hide_fieldset' => FALSE,
  );
  $t_color_options = array(
    'settings' => empty($settings['color']['top']) ? FALSE : $settings['color']['top'],
    'color_field_title' => t('Border top'),
    'id' => $id . '_top',
    'fields_id' => $id . '--color--top--',
    'default' => 'Border default',
    'hide_fieldset' => FALSE,
  );
  $b_color_options = array(
    'settings' => empty($settings['color']['bottom']) ? FALSE : $settings['color']['bottom'],
    'color_field_title' => t('Border bottom'),
    'id' => $id . '_bottom',
    'fields_id' => $id . '--color--bottom--',
    'default' => 'Border default',
    'hide_fieldset' => FALSE,
  );
  $color_fields[] = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => $different_color,
  ));
  $color_fields[] = style_manager_color(array('data' => $default_color_options));
  $color_fields[] = style_manager_color(array('data' => $t_color_options));
  $color_fields[] = style_manager_color(array('data' => $r_color_options));
  $color_fields[] = style_manager_color(array('data' => $b_color_options));
  $color_fields[] = style_manager_color(array('data' => $l_color_options));
  $tabs = array();
  $tabs[] = array(
    'title' => t('Border options'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => _style_manager_jsform_fieldset(array(
      'title' => '',
      'collapsible' => FALSE,
      'items' => $items,
    )),
  );
  $tabs[] = array(
    'title' => t('Border color'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => $color_fields,
  );

  $settings['top_left'] = isset($settings['border_radius']['top_left']) ? $settings['border_radius']['top_left'] : 'none';
  $settings['top_right'] = isset($settings['border_radius']['top_right']) ? $settings['border_radius']['top_right'] : 'none';
  $settings['bottom_right'] = isset($settings['border_radius']['bottom_right']) ? $settings['border_radius']['bottom_right'] : 'none';
  $settings['bottom_left'] = isset($settings['border_radius']['bottom_left']) ? $settings['border_radius']['bottom_left'] : 'none';

  $options = array('-', array('0', 'none', 'none'));
  $options_tr = $options;
  $options_tl = $options;
  $options_br = $options;
  $options_bl = $options;

  $options_tr[] = $options_bl[] = $options_br[] = array(
    'copy--top_left',
    'As T-L',
    'As Top-left',
  );
  $options_tl[] = $options_bl[] = $options_br[] = array(
    'copy--top_right',
    'As T-R',
    'As Top-right',
  );
  $options_tl[] = $options_tr[] = $options_br[] = array(
    'copy--bottom_left',
    'As B-L',
    'As Bottom-left',
  );
  $options_tl[] = $options_tr[] = $options_bl[] = array(
    'copy--bottom_right',
    'As B-R',
    'As Bottom-right',
  );

  $items = array();
  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--border_radius--top_left',
      'value' => $settings['border_radius']['top_left'],
      'options' => $options_tl,
    )),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--border_radius--top_right',
      'value' => $settings['border_radius']['top_right'],
      'title' => t('Top-right'),
      'options' => $options_tr,
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Top-left'),
    'items' => $fields,
  ));

  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--border_radius--bottom_left',
      'value' => $settings['border_radius']['bottom_left'],
      'options' => $options_bl,
    )),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--border_radius--bottom_right',
      'value' => $settings['border_radius']['bottom_right'],
      'title' => t('Bottom-right'),
      'options' => $options_br,
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Bottom-left'),
    'items' => $fields,
  ));

  $tabs[] = array(
    'title' => t('Border radius'),
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => _style_manager_jsform_fieldset(array(
      'title' => '',
      'collapsible' => FALSE,
      'items' => $items,
    )),
  );

  $tabpanel = _style_manager_jsform_tabpanel(array('items' => $tabs));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $tabpanel,
  ));
}


/**
 * Type Box.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_box($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['display'] = isset($settings['display']) ? $settings['display'] : '-';
  $settings['clear'] = isset($settings['clear']) ? $settings['clear'] : '-';
  $settings['float'] = isset($settings['float']) ? $settings['float'] : '-';
  $settings['overflow'] = isset($settings['overflow']) ? $settings['overflow'] : '-';
  $settings['height'] = isset($settings['height']) ? $settings['height'] : 'auto';
  $settings['width'] = isset($settings['width']) ? $settings['width'] : 'auto';

  $items = array();

  $display_options = array(
    '-',
    'none',
    'block',
    'inline',
    'inline-block',
    'inline-table',
    'list-item',
    'run-in',
    'table',
    'table-caption',
    'table-cell',
    'table-column-group',
    'table-column',
    'table-footer-group',
    'table-header-group',
    'table-row',
    'table-row-group',
  );
  $display_field = _style_manager_field_combo(array(
    'id' => $id . '--display',
    'width' => 280,
    'val' => $settings['display'],
    'options' => $display_options,
    'title' => t('Display'),
  ));
  $overflow_options = array(
    '-',
    'auto',
    'hidden',
    'scroll',
    'visible',
    'inherit',
  );
  $overflow_field = _style_manager_field_combo(array(
    'id' => $id . '--overflow',
    'width' => 280,
    'val' => $settings['overflow'],
    'options' => $overflow_options,
    'title' => t('Overflow'),
  ));
  $float_options = array('-', 'none', 'right', 'left');
  $float_field = _style_manager_field_combo(array(
    'id' => $id . '--float',
    'width' => 185,
    'val' => $settings['float'],
    'options' => $float_options,
    'title' => t('Float / Clear'),
  ));
  $clear_options = array('-', 'none', 'right', 'left', 'both');
  $clear_field = _style_manager_field_combo(array(
    'id' => $id . '--clear',
    'width' => 90,
    'val' => $settings['clear'],
    'options' => $clear_options,
    'title' => '',
  ));
  $height = _style_manager_jsform_advanced_numeric(array(
    'id' => $id . '--height',
    'value' => $settings['height'],
    'options' => array('-', 'auto'),
  ));
  $width = _style_manager_jsform_advanced_numeric(array(
    'id' => $id . '--width',
    'value' => $settings['width'],
    'options' => array('-', 'auto'),
  ));

  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Height'),
    'items' => array($height, $display_field),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Width'),
    'items' => array($width, $overflow_field),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => array(
      array('xtype' => 'displayfield', 'width' => 205),
      $float_field,
      $clear_field,
    ),
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => array(array('xtype' => 'displayfield', 'width' => 205)),
  ));

  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}


/**
 * Type Position.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_position($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';

  $placement_settings = array();
  $placement_settings['data']['settings'] = $settings['placement'];
  $placement_settings['data']['id'] = $id . '--placement';
  $items = _style_manager_box_indent($placement_settings, array('-', 'auto'));
  $settings['position'] = isset($settings['position']) ? $settings['position'] : '-';

  $position_options = array('-', 'absolute', 'fixed', 'relative', 'static');
  $position_field = _style_manager_field_combo(array(
    'id' => $id . '--position',
    'width' => 205,
    'val' => $settings['position'],
    'options' => $position_options,
  ));
  $checkboxes = array(
    $position_field,
    array(
      'xtype' => 'numberfield',
      'name' => $id . '--z_index',
      'fieldLabel' => t('Z-index'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 95,
      'width' => 200,
      'minValue' => 0,
      'step' => 5,
      'allowBlank' => TRUE,
      'value' => isset($settings['z_index']) ? $settings['z_index'] : '',
    ),
  );
  $position_settings = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Position'),
    'items' => $checkboxes,
    'margin' => '0 0 5px 0',
  ));
  array_unshift($items, $position_settings);

  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}

/**
 * Type CSS code.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_css_code($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $tabs = array();

  // Raw CSS.
  $css_code_tab = array();
  $css_code_tab[] = array(
    'xtype' => 'displayfield',
    'margin' => '0',
    'value' => 'CSS code:',
  );
  $css_code_tab[] = array(
    'xtype' => 'textarea',
    'id' => $id . '--css_code',
    'name' => $id . '--css_code',
    'value' => $settings['css_code'],
    'height' => 150,
    'margin' => '0 0 5px 0',
    'width' => 610,
  );
  $css_code_tab[] = array(
    'xtype' => 'button',
    'margin' => '0',
    'text' => t('Import CSS'),
    'scale' => 'medium',
    'handler' => "<js_start> function(){ Drupal.style_manager.import_css_code('{$id}--css_code'); } <js_end>",
    'margin' => '0 0 5px 0',
    'minWidth' => 90,
  );
  $tabs[] = array(
    'title' => t('Raw CSS'),
    'padding' => '10px',
    'frame' => TRUE,
    'items' => $css_code_tab,
  );

  // Settings.
  $css_prefix = array(
    'xtype' => 'checkboxfield',
    'margin' => '0',
    'boxLabel' => t('Add css prefix to all selectors'),
    'checked' => !isset($settings['add_css_prefix']) ? TRUE : $settings['add_css_prefix'],
    'name' => $id . '--add_css_prefix',
  );
  $settings_tab[] = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => $css_prefix,
    'margin' => '0 0 5px 0',
  ));

  $color_overriding = array(
    'xtype' => 'checkboxfield',
    'margin' => '0',
    'boxLabel' => t('Color overriding'),
    'checked' => !isset($settings['color_overriding']) ? FALSE : $settings['color_overriding'],
    'listeners' => "<js_start> { change: function(el, value){ Drupal.style_manager.form_listeners.css_code.color_overriding.change('{$id}', value); } } <js_end>",
    'name' => $id . '--color_overriding',
  );
  $settings_tab[] = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => $color_overriding,
    'margin' => '0 0 5px 0',
  ));

  $images_overriding = array(
    'xtype' => 'checkboxfield',
    'margin' => '0',
    'boxLabel' => t('Images overriding'),
    'checked' => !isset($settings['images_overriding']) ? TRUE : $settings['images_overriding'],
    'name' => $id . '--images_overriding',
    'id' => $id . '_images_overriding',
    'disabled' => empty($settings['color_overriding']),
  );
  $settings_tab[] = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => $images_overriding,
    'margin' => '0 0 5px 0',
  ));

  $tabs[] = array(
    'title' => t('Settings'),
    'padding' => '10px 0 10px 0',
    'frame' => TRUE,
    'items' => $settings_tab,
  );

  // Color overriding.
  for ($i = 1; $i < STYLE_MANAGER_CSS_CODE_COLORS; $i++) {
    $tabs[] = style_manager_color_overriding_tab($id, $i, $settings);
  }

  $tabpanel = _style_manager_jsform_tabpanel(array('items' => $tabs));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $tabpanel,
  ));
}


/**
 * Type Box shadow.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_box_shadow($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['blur'] = isset($settings['blur']) ? $settings['blur'] : '';
  $settings['size'] = isset($settings['size']) ? $settings['size'] : '';

  $items = array();
  $checkboxes = array(
    array(
      'xtype' => 'checkboxfield',
      'margin' => '0 0 5px 0px',
      'boxLabel' => t('Disable shadow'),
      'checked' => !empty($settings['disable']) ? TRUE : FALSE,
      'name' => $id . '--disable',
    ),
    array(
      'xtype' => 'checkboxfield',
      'margin' => '0 0 0 10px',
      'boxLabel' => t('Inner shadow'),
      'checked' => !empty($settings['inset']) ? TRUE : FALSE,
      'name' => $id . '--inset',
    ),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array('items' => $checkboxes));
  $shadow_settings = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--offset_x',
      'value' => $settings['offset_x'],
    )),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--size',
      'value' => $settings['size'],
      'title' => t('Size'),
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Offset (X)'),
    'items' => $shadow_settings,
  ));

  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--offset_y',
      'value' => $settings['offset_y'],
    )),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--blur',
      'value' => $settings['blur'],
      'title' => t('Blur'),
      'min' => 0,
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Offset (Y)'),
    'items' => $fields,
  ));

  $color_options = array(
    'settings' => empty($settings['color']) ? FALSE : $settings['color'],
    'id' => $id,
    'fields_id' => $id . '--color--',
    'hide_fieldset' => TRUE,
  );

  $color = style_manager_color(array('data' => $color_options));
  $tabs = array();
  $tabs[] = array(
    'title' => 'Shadow options',
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => _style_manager_jsform_fieldset(array(
      'title' => '',
      'collapsible' => FALSE,
      'items' => $items,
    )),
  );
  $tabs[] = array(
    'title' => 'Shadow color',
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => _style_manager_jsform_fieldset(array(
      'title' => '',
      'collapsible' => FALSE,
      'items' => $color,
    )),
  );
  $tabpanel = _style_manager_jsform_tabpanel(array('items' => $tabs));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $tabpanel,
  ));
}


/**
 * Type Margin.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_margin($variables) {
  $variables['data']['id'] .= '--settings';
  $items = _style_manager_box_indent($variables, array(
    '-',
    array('0', 'none', 'none'),
    'auto',
  ));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}

/**
 * Type Padding.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_padding($variables) {
  $variables['data']['id'] .= '--settings';
  $items = _style_manager_box_indent($variables, array(
    '-',
    array('0', 'none', 'none'),
  ));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}


/**
 * Type Background.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_background($variables) {
  $data = $variables['data'];
  $settings = $data['settings'];
  $id = $data['id'] . '--settings';

  // Background color.
  $background_color_options = array(
    'settings' => empty($settings['color']) ? FALSE : $settings['color'],
    'id' => $id,
    'fields_id' => $id . '--color--',
    'hide_fieldset' => TRUE,
  );
  $background_color_items = style_manager_color(array('data' => $background_color_options));
  $transparent_checkbox = array(
    'xtype' => 'checkboxfield',
    'margin' => '0 0 10px 100px',
    'boxLabel' => t('Transparent'),
    'checked' => !empty($settings['color']['transparent']) ? TRUE : FALSE,
    'name' => $id . '--color--transparent',
  );
  array_unshift($background_color_items, $transparent_checkbox);
  $background_color = _style_manager_jsform_fieldset(array(
    'title' => '',
    'collapsible' => FALSE,
    'items' => $background_color_items,
  ));

  $tabs[] = array(
    'title' => 'Background color',
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => $background_color,
  );

  // Background image.
  $image_field_options = array('-', 'none');
  if (!empty($settings['image']['url']) && !in_array($settings['image']['url'], $image_field_options)) {
    $image_field_options[] = $settings['image']['url'];
  }
  $output_selectors = implode(',', $data['output_selectors']);
  $output_selectors = str_replace(array(
    ':link',
    ':hover',
    ':visited',
    ':active',
  ), '', $output_selectors);
  $image_field_afterrender = "function(el, value){ Drupal.style_manager.form_listeners.background.image_url.afterrender('{$id}', '{$output_selectors}'); }";
  $image_field = _style_manager_field_combo(array(
    'id' => $id . '--image--url',
    'width' => 402,
    'val' => !empty($settings['image']['url']) ? $settings['image']['url'] : '',
    'options' => $image_field_options,
    'editable' => TRUE,
    'afterrender' => $image_field_afterrender,
  ));
  $background_image_items = array();
  $image_url_field = array(
    $image_field,
    array(
      'text' => module_invoke('imce', 'access') ? t('Browse') : t('no imce'),
      'width' => 95,
      'xtype' => 'button',
      'stateful' => FALSE,
      'disabled' => !module_invoke('imce', 'access'),
      'handler' => "<js_start> function() { Drupal.style_manager.imce_browser_open('{$id}--image--url'); } <js_end>",
    ),
  );
  $background_image_items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => 'Image url',
    'items' => $image_url_field,
  ));

  $background_repeat_options = array(
    '-',
    'no-repeat',
    'repeat',
    'repeat-x',
    'repeat-y',
  );
  $background_repeat_field = _style_manager_field_combo(array(
    'id' => $id . '--image--repeat',
    'val' => $settings['image']['repeat'],
    'options' => $background_repeat_options,
    'width' => 176,
  ));

  $background_attachment_options = array('-', 'fixed', 'scroll');
  $background_attachment_field = _style_manager_field_combo(array(
    'id' => $id . '--image--attachment',
    'val' => $settings['image']['attachment'],
    'options' => $background_attachment_options,
    'width' => 176,
  ));

  $background_position_x_field = _style_manager_jsform_advanced_numeric(array(
    'value' => $settings['image']['position-x'],
    'id' => $id . '--image--position-x',
    'options' => array('-', 'left', 'center', 'right'),
  ));
  $background_position_y_field = _style_manager_jsform_advanced_numeric(array(
    'value' => $settings['image']['position-y'],
    'id' => $id . '--image--position-y',
    'options' => array('-', 'top', 'center', 'bottom'),
  ));

  $background_position = array(
    $background_position_x_field,
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => 'Repeat',
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 95,
      'margin' => '0 3px 0 5px',
    ),
    $background_repeat_field,
  );
  $repeat_attachment = array(
    $background_position_y_field,
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => 'Attachment',
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 95,
      'margin' => '0 3px 0 5px',
    ),
    $background_attachment_field,
  );
  $background_image_items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => 'Position (Y)',
    'items' => $repeat_attachment,
  ));
  $background_image_items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => 'Position (X)',
    'items' => $background_position,
  ));

  $background_image = _style_manager_jsform_fieldset(array(
    'title' => '',
    'collapsible' => FALSE,
    'items' => $background_image_items,
  ));

  $tabs[] = array(
    'title' => 'Image',
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => $background_image,
  );

  // Image settings.
  $brightness = array(
    'xtype' => 'numberfield',
    'name' => $id . '--image_post_processing--brightness',
    'id' => $id . '_image_post_processing_brightness',
    'width' => 60,
    'maxValue' => 255,
    'minValue' => -255,
    'step' => 5,
    'allowBlank' => TRUE,
    'value' => isset($settings['image_post_processing']['brightness']) ? $settings['image_post_processing']['brightness'] : '0',
  );
  $contrast = array(
    'xtype' => 'numberfield',
    'name' => $id . '--image_post_processing--contrast',
    'id' => $id . '_image_post_processing_contrast',
    'width' => 60,
    'maxValue' => 255,
    'minValue' => -255,
    'step' => 5,
    'allowBlank' => TRUE,
    'value' => isset($settings['image_post_processing']['contrast']) ? $settings['image_post_processing']['contrast'] : '0',
  );
  $transparency = array(
    'xtype' => 'numberfield',
    'name' => $id . '--image_post_processing--transparency',
    'id' => $id . '_image_post_processing_transparency',
    'width' => 60,
    'maxValue' => 127,
    'minValue' => -127,
    'step' => 5,
    'allowBlank' => TRUE,
    'value' => isset($settings['image_post_processing']['transparency']) ? $settings['image_post_processing']['transparency'] : '0',
  );
  $image_options = array(
    $contrast,
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => t('Brightness'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 70,
      'margin' => '0',
    ),
    $brightness,
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => t('Transparence'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 85,
      'margin' => '0',
    ),
    $transparency,
  );
  $img_settings_container = _style_manager_jsform_fieldcontainer(array(
    'title' => 'Contrast',
    'items' => $image_options,
    'margin' => '0 0 5px 0',
    'id' => $id . '_image_options',
    'disabled' => empty($settings['color_overriding']),
  ));

  $color_overriding = array(
    'xtype' => 'checkboxfield',
    'margin' => '0 0 5px 0',
    'boxLabel' => t('Overriding image'),
    'checked' => !isset($settings['color_overriding']) ? FALSE : $settings['color_overriding'],
    'listeners' => "<js_start> { change: function(el, value){ Drupal.style_manager.form_listeners.css_code.color_overriding.change('{$id}', value); } } <js_end>",
    'name' => $id . '--color_overriding',
    'id' => $id . '_color_overriding',
  );
  $image_overriding_container = _style_manager_jsform_fieldcontainer(array(
    'title' => '',
    'items' => $color_overriding,
    'margin' => '0',
  ));

  $img_processing_color = _style_manager_jsform_fieldset(array(
    'title' => '',
    'collapsible' => FALSE,
    'padding' => '5px 5px 5px 5px',
    'margin' => '5px 5px 10px 5px',
    'items' => array($image_overriding_container, $img_settings_container),
  ));

  $tabs[] = array(
    'title' => 'Image post processing',
    'padding' => '0',
    'frame' => TRUE,
    'items' => array($img_processing_color),
  );

  // Color overriding.
  for ($i = 1; $i < 4; $i++) {
    $tabs[] = style_manager_color_overriding_tab($id, $i, $settings);
  }

  $field = _style_manager_jsform_tabpanel(array('items' => $tabs));
  return _style_manager_jsform_fieldset(array(
    'title' => $data['title'],
    'items' => $field,
  ));
}

/**
 * Type Global font.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_global_font($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['font'] = isset($settings['font']) ? $settings['font'] : 'Verdana, Geneva, sans-serif';
  $items = array();

  $font_options = array();
  $font_options[] = 'Verdana, Geneva, sans-serif';
  $font_options[] = 'Georgia, "Times New Roman", Times, serif';
  $font_options[] = 'Courier New, Courier, monospace';
  $font_options[] = 'Arial, Helvetica, sans-serif';
  $font_options[] = 'Tahoma, Geneva, sans-serif';
  $font_options[] = '"Trebuchet MS", Arial, Helvetica, sans-serif';
  $font_options[] = '"Arial Black", Gadget, sans-serif';
  $font_options[] = '"Times New Roman", Times, serif';
  $font_options[] = '"Palatino Linotype", "Book Antiqua", Palatino, serif';
  $font_options[] = '"Lucida Sans Unicode", "Lucida Grande", sans-serif';
  $font_options[] = '"MS Serif", "New York", serif';
  $font_options[] = '"Lucida Console", Monaco, monospace';

  $font_field = _style_manager_field_combo(array(
    'id' => $id . '--font',
    'width' => 495,
    'val' => $settings['font'],
    'options' => $font_options,
    'editable' => TRUE,
  ));
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Font'),
    'items' => array($font_field),
  ));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}

/**
 * Type Global_numeric.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_global_numeric($variables) {
  $settings = $variables['data']['settings'];
  $id = $variables['data']['id'] . '--settings';
  $settings['value'] = isset($settings['value']) ? $settings['value'] : array('str_value' => '-');
  $options = array('-');
  $global_numeric = style_manager_get_globals_var('global_numeric');
  if ($global_numeric && empty($variables['hide_globals'])) {
    foreach ($global_numeric as $key => $var) {
      if ($key == $variables['data']['id']) {
        continue;
      }
      $options['global--' . $key] = array(
        'global--' . $key,
        strip_tags(t('g:') . $var['title']),
        t('g:') . $var['title'],
      );
    }
  }
  $items = array();
  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--value',
      'value' => $settings['value'],
      'options' => $options,
      'hide_globals' => TRUE,
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Value'),
    'items' => $fields,
  ));
  return _style_manager_jsform_fieldset(array(
    'title' => $variables['data']['title'],
    'collapsible' => FALSE,
    'items' => $items,
  ));
}


/**
 * Type global_main_color.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_global_color($variables) {
  $data = $variables['data'];
  $id = $data['id'] . '--settings--';
  if (!empty($data['fields_id'])) {
    $id = $data['fields_id'];
  }
  $fieldcontainer_items = array();
  $main_color = FALSE;
  if ($data['id'] == 'main_color') {
    $main_color = TRUE;
  }

  $default = array(
    'based_on' => 'main_color',
    'color_correct' => FALSE,
    'hue' => array('+', ''),
    'saturation' => array('=', ''),
    'lightness' => array('=', ''),
    'hex' => '',
  );
  if (empty($data['settings'])) {
    $settings = $default;
  }
  else {
    $settings = $data['settings'] + $default;
  }
  $settings['id'] = $id;

  if ($main_color) {
    $items = _style_manager_field_colorpicker(array(
      'val' => $settings['hex'],
      'id' => $id . 'hex',
    ));
    $fieldcontainer_items[] = _style_manager_jsform_fieldcontainer(array(
      'title' => t('Hex'),
      'items' => $items,
    ));
  }
  else {
    $items = _style_manager_jsform_base_color(array(
      'data' => $settings,
      'global_id' => $data['id'],
    ));
    $fieldcontainer_items[] = _style_manager_jsform_fieldcontainer(array(
      'title' => 'Color',
      'items' => $items,
    ));
    $items = _style_manager_jsform_hsl(array('data' => $settings));
    $fieldcontainer_items[] = _style_manager_jsform_fieldcontainer(array(
      'title' => t('Hue'),
      'items' => $items,
      'margin' => '5px 0 0 0',
      'id' => $id . '_correct',
    ));
  }

  return _style_manager_jsform_fieldset(array(
    'title' => $data['title'],
    'items' => $fieldcontainer_items,
  ));
}

/**
 * Type Color.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_color($variables) {
  $data = $variables['data'];
  $id = $data['id'] . '--settings--';
  if (!empty($data['fields_id'])) {
    $id = $data['fields_id'];
  }
  $fieldcontainer_items = array();

  $default = array(
    'based_on' => 'main_color',
    'color_correct' => FALSE,
    'hue' => array('+', ''),
    'saturation' => array('=', ''),
    'lightness' => array('=', ''),
    'hex' => '',
  );
  if (empty($data['settings'])) {
    $settings = $default;
  }
  else {
    $settings = (array) $data['settings'] + $default;
  }
  $settings['id'] = $id;

  $items = _style_manager_jsform_base_color(array('data' => $settings));
  $color_field_title = empty($data['color_field_title']) ? t('Color') : $data['color_field_title'];
  $fieldcontainer_items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => $color_field_title,
    'items' => $items,
  ));

  $items = _style_manager_jsform_hsl(array('data' => $settings));
  $fieldcontainer_items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Hue'),
    'items' => $items,
    'margin' => '5px 0 0 0',
    'id' => $id . '_correct',
  ));

  if (!empty($data['hide_fieldset'])) {
    $out = $fieldcontainer_items;
  }
  else {
    $out = _style_manager_jsform_fieldset(array(
      'title' => $data['title'],
      'items' => $fieldcontainer_items,
      'id' => 'fieldset_' . $data['id'],
    ));
  }
  return $out;
}

/**
 * Ext form fieldset.
 */
function _style_manager_jsform_fieldset($variables) {
  $title = $variables['title'];
  $items = $variables['items'];
  $collapsible = empty($variables['collapsible']) ? FALSE : TRUE;
  $collapsed = empty($variables['collapsed']) ? FALSE : TRUE;
  if ($title) {
    $collapsible = TRUE;
    $collapsed = TRUE;
  }
  $out = array(
    'xtype' => 'fieldset',
    'cls' => empty($variables['cls']) ? 'jsform_fieldset' : $variables['cls'],
    'title' => $title,
    'collapsible' => $collapsible,
    'collapsed' => $collapsed,
    'padding' => '10px 5px 5px 5px',
    'margin' => '5px 5px 10px 5px',
    'defaults' => array(
      'labelWidth' => 95,
      'anchor' => '100%',
      'margin' => '10',
      'stateful' => FALSE,
      'layout' => array(
        'type' => 'hbox',
        'defaultMargins' => array(
          'top' => 0,
          'right' => 5,
          'bottom' => 0,
          'left' => 0,
        ),
      ),
    ),
    'items' => $items,
  );
  $additional_fields = array(
    'id',
    'hidden',
    'disabled',
    'margin',
    'margin',
    'padding',
  );
  foreach ($additional_fields as $field_key) {
    if (isset($variables[$field_key])) {
      $out[$field_key] = $variables[$field_key];
    }
  }
  return $out;
}

/**
 * Ext form fieldcontainer.
 */
function _style_manager_jsform_fieldcontainer($variables) {
  $title = isset($variables['title']) ? $variables['title'] : '';
  $items = $variables['items'];

  $out = array(
    'xtype' => 'fieldcontainer',
    'anchor' => '100%',
    'labelAlign' => 'right',
    'margin' => 0,
    'fieldLabel' => $title,
    'hideEmptyLabel' => FALSE,
    'defaults' => array('hideLabel' => TRUE, 'stateful' => FALSE),
    'items' => $items,
  );
  $additional_fields = array('id', 'hidden', 'disabled', 'margin');
  foreach ($additional_fields as $field_key) {
    if (!empty($variables[$field_key])) {
      $out[$field_key] = $variables[$field_key];
    }
  }
  return $out;
}

/**
 * Ext form fieldcontainer.
 */
function _style_manager_jsform_tabpanel($variables) {
  $items = $variables['items'];
  foreach ($items as &$data) {
    if (isset($data['items']['margin'])) {
      $data['items']['margin'] = '0';
    }
    if (isset($data['items']['padding'])) {
      $data['items']['padding'] = '10px 5px 5px 5px';
    }
  }
  $out = array(
    'xtype' => 'tabpanel',
    'cls' => 'style_manager_tabpanel',
    'plain' => TRUE,
    'anchor' => '100%',
    'padding' => '0',
    'margin' => '0',
    'items' => $items,
  );
  return $out;
}

/**
 * Ext form fields hsl.
 */
function _style_manager_jsform_hsl($variables) {
  $out = array();
  $data = $variables['data'];
  $options = array(
    array('=', '=', '='),
    array('+', '+', '+'),
    array('-', '-', '-'),
  );
  $hsl_types = array(
    'hue' => t('Hue'),
    'saturation' => '<span title="Saturation">S</span>',
    'lightness' => '<span title="Lightness">L</span>',
  );
  foreach ($hsl_types as $type => $title) {
    $combo_options = array(
      'id' => $data['id'] . $type . '--0',
      'val' => $data[$type][0],
      'options' => $options,
      'width' => 40,
      'stateful' => FALSE,
    );

    if ($type != 'hue') {
      $out[] = array(
        'xtype' => 'displayfield',
        'margin' => '0 2px 0 9px',
        'value' => $title . ':',
        'stateful' => FALSE,
      );
    }
    $out[] = _style_manager_field_combo($combo_options);
    $out[] = array(
      'xtype' => 'numberfield',
      'name' => $data['id'] . $type . '--1',
      'width' => 60,
      'maxValue' => $type == 'hue' ? 359 : 100,
      'minValue' => 0,
      'allowBlank' => TRUE,
      'value' => $data[$type][1],
      'stateful' => FALSE,
    );
  }
  return $out;
}

/**
 * Ext form combo.
 */
function _style_manager_field_combo($variables) {
  $id = $variables['id'];
  $val = $variables['val'];
  $options = $variables['options'];

  if (!empty($options)) {
    foreach ($options as &$data) {
      if (is_array($data)) {
        $data = array(
          'value' => $data[0],
          'name' => strip_tags($data[1]),
          'name_html' => $data[2],
          'description' => isset($data[3]) ? $data[3] : '',
        );
      }
      else {
        $data = array(
          'value' => $data,
          'name' => $data,
          'name_html' => $data,
          'description' => '',
        );
      }
    }
  }
  $options = array_values($options);
  $out = array(
    'flex' => 1,
    'xtype' => 'combo',
    'mode' => 'local',
    'queryMode' => 'local',
    'value' => $val,
    'triggerAction' => 'all',
    'editable' => empty($variables['editable']) ? FALSE : TRUE,
    'id' => $id,
    'name' => $id,
    'displayField' => 'name',
    'valueField' => 'value',
    'stateful' => FALSE,
    'store' => array(
      'fields' => array('value', 'name', 'name_html', 'description'),
      'data' => $options,
    ),
    'listConfig' => array(
      'minWidth' => !empty($variables['min_width']) ? $variables['min_width'] : 0,
      'maxWidth' => 1000,
      'getInnerTpl' => "<js_start> function() { return '<div  data-qtip=\"{description}\" style=\"white-space: nowrap;\">{name_html}</div>';} <js_end>",
    ),
  );
  if (empty($variables['listeners'])) {
    $variables['listeners'] = 'function(){}';
  }
  if (empty($variables['afterrender'])) {
    $variables['afterrender'] = 'function(){}';
  }
  if (!empty($variables['width'])) {
    unset($out['flex']);
    $out['width'] = $variables['width'];
  }
  if (!empty($variables['title'])) {
    $out['fieldLabel'] = $variables['title'];
    $out['labelAlign'] = 'right';
    $out['hideLabel'] = FALSE;
    $out['labelWidth'] = 95;
  }

  $variables['listeners'] = str_replace(array(
    "\r",
    "\n",
  ), array(
    "",
    "",
  ), $variables['listeners']);
  $variables['afterrender'] = str_replace(array(
    "\r",
    "\n",
  ), array(
    "",
    "",
  ), $variables['afterrender']);
  $out['listeners'] = "<js_start> { change: {$variables['listeners']}, afterrender: {$variables['afterrender']} } <js_end>";

  return $out;
}

/**
 * Ext form colorpicker.
 */
function _style_manager_field_colorpicker($variables) {
  $id = $variables['id'];
  $val = $variables['val'];
  $out = array();
  if (empty($variables['hide_prefix'])) {
    $out[] = array(
      'xtype' => 'displayfield',
      'value' => '#',
    );
  }
  $out[] = array(
    'flex' => 1,
    'xtype' => 'textfield',
    'stateful' => FALSE,
    'cls' => 'colorpickerfield',
    'value' => str_replace('#', '', $val),
    'name' => $id,
  );
  return $out;
}

/**
 * Global colors.
 */
function _style_manager_jsform_base_color($variables) {
  $data = $variables['data'];

  $options = array();
  $colors = style_manager_get_globals_var('global_color');
  $options[] = array('-', '-', '-');
  $options[] = array('hex', t('Custom (hex)'), t('Custom (hex)'));
  foreach ($colors as $key => $var) {
    if (!empty($variables['global_id']) && $variables['global_id'] == $key) {
      continue;
    }
    $options[] = array($key, strip_tags($var['title']), $var['title']);
  }
  $listeners = " function() {
                      var based_on = Ext.getCmp('{$data['id']}based_on');
                      var color_correct = Ext.getCmp('{$data['id']}_color_correct');
                      if (based_on.value == 'hex') {
                        Ext.getCmp('{$data['id']}_colorpicker_prefix').show();
                        Ext.getCmp('{$data['id']}_colorpickerfield').show();

                        Ext.getCmp('{$data['id']}_color_correct').hide();
                        Ext.getCmp('{$data['id']}_correct').hide();
                      }
                      else if (based_on.value == '-') {
                        Ext.getCmp('{$data['id']}_colorpicker_prefix').hide();
                        Ext.getCmp('{$data['id']}_colorpickerfield').hide();
                        Ext.getCmp('{$data['id']}_color_correct').hide();
                        Ext.getCmp('{$data['id']}_correct').hide();
                      }
                      else {
                        Ext.getCmp('{$data['id']}_colorpicker_prefix').hide();
                        Ext.getCmp('{$data['id']}_colorpickerfield').hide();
                        Ext.getCmp('{$data['id']}_color_correct').show();
                        if (color_correct.value) {
                          Ext.getCmp('{$data['id']}_correct').show();
                        }
                        else {
                          Ext.getCmp('{$data['id']}_correct').hide();
                        }
                      }
                 }
  ";
  $listeners = str_replace(array("\r", "\n"), array("", ""), $listeners);
  $combo_options = array(
    'id' => $data['id'] . 'based_on',
    'val' => $data['based_on'],
    'options' => $options,
    'listeners' => $listeners,
    'afterrender' => $listeners,
    'width' => 390,
  );
  $field_combo = _style_manager_field_combo($combo_options);

  return array(
    $field_combo,
    array(
      'id' => $data['id'] . '_colorpicker_prefix',
      'xtype' => 'displayfield',
      'value' => '#',
    ),
    array(
      'id' => $data['id'] . '_colorpickerfield',
      'width' => 100,
      'xtype' => 'textfield',
      'cls' => 'colorpickerfield',
      'value' => str_replace('#', '', $data['hex']),
      'name' => $data['id'] . 'hex',
    ),
    array(
      'id' => $data['id'] . '_color_correct',
      'xtype' => 'checkboxfield',
      'width' => 107,
      'margin' => '0 0 1px 6px',
      'boxLabel' => t('Correct color'),
      'checked' => $data['color_correct'],
      'name' => $data['id'] . 'color_correct',
      'listeners' => "<js_start> { change: {$listeners} } <js_end>",
    ),
  );
}

/**
 * Advansed numeric fieldgroup.
 */
function _style_manager_jsform_advanced_numeric($variables) {
  $value_combo_options_raw = isset($variables['options']) ? $variables['options'] : FALSE;
  $value_combo_options = array();
  $id = $variables['id'];
  $unit_options = array('px', '%', 'pt', 'in', 'cm', 'mm', 'pc', 'em', 'ex');

  $value_combo_value = isset($variables['value']['str_value']) ? $variables['value']['str_value'] : 'value';
  $value_number_value = isset($variables['value']['int_value']) ? $variables['value']['int_value'] : '';
  $unit_combo_value = isset($variables['value']['int_value_unit']) ? $variables['value']['int_value_unit'] : 'px';
  $global_unit_combo_value = isset($variables['value']['int_value_global_unit']) ? $variables['value']['int_value_global_unit'] : 'val';
  $title = isset($variables['title']) ? $variables['title'] : '';
  $fields = array();

  if ($title) {
    $fields[] = array(
      'xtype' => 'displayfield',
      'fieldLabel' => $title,
      'labelAlign' => 'right',
      'labelWidth' => 95,
      'hideLabel' => FALSE,
      'margin' => '0',
    );
  }
  if ($value_combo_options_raw) {
    $global_numeric = style_manager_get_globals_var('global_numeric');
    foreach ($value_combo_options_raw as $var) {
      if (is_array($var)) {
        $value_combo_options[$var[0]] = $var;
      }
      else {
        $value_combo_options[$var] = array($var, $var, $var);
      }
    }

    if ($global_numeric && empty($variables['hide_globals'])) {
      foreach ($global_numeric as $key => $var) {
        $value_combo_options['global--' . $key] = array(
          'global--' . $key,
          strip_tags(t('g:') . $var['title']),
          t('g:') . $var['title'],
        );
      }
    }
    $listeners = " function(el, old_val, new_val) {
                      var str_value = Ext.getCmp('{$id}--str_value');
                      var disabled = true;
                      if (str_value.value == 'value') {
                        disabled = false;
                      }
                      if (Drupal.style_manager.advanced_numeric_store['{$id}--int_value'] && new_val) {
                        Ext.getCmp('{$id}--int_value').setValue(Drupal.style_manager.advanced_numeric_store['{$id}--int_value']);
                      }
                      Ext.getCmp('{$id}--int_value').setDisabled(disabled);
                      Ext.getCmp('{$id}--int_value_unit').setDisabled(disabled);
                      str_value_arr = str_value.value.split('--');
                      if (str_value_arr[0] == 'global') {
                         Ext.getCmp('{$id}--int_value_unit').hide();
                         Ext.getCmp('{$id}--int_value_global_unit').show();
                         Ext.getCmp('{$id}--int_value').setDisabled(false);
                         if (new_val) {
                           Drupal.style_manager.advanced_numeric_store['{$id}--int_value'] = Ext.getCmp('{$id}--int_value').getValue();
                           Ext.getCmp('{$id}--int_value').setValue(0);
                         }
                      }
                      else {
                         Ext.getCmp('{$id}--int_value_unit').show();
                         Ext.getCmp('{$id}--int_value_global_unit').hide();
                      }
                 }
    ";
    $value_combo_options['value'] = array('value', '-value-', '-value-');
    if (!isset($value_combo_options[$value_combo_value])) {
      $value_combo_value = 'value';
    }
    $combo_options = array(
      'id' => $id . '--str_value',
      'val' => $value_combo_value,
      'options' => $value_combo_options,
      'listeners' => $listeners,
      'afterrender' => $listeners,
      'width' => 75,
      'min_width' => 150,
    );
    $fields[] = _style_manager_field_combo($combo_options);
  }

  $fields[] = array(
    'xtype' => 'numberfield',
    'name' => $id . '--int_value',
    'id' => $id . '--int_value',
    'minValue' => isset($variables['min']) ? $variables['min'] : FALSE,
    'width' => 75,
    'allowBlank' => TRUE,
    'value' => $value_number_value,
  );
  $fields[] = _style_manager_field_combo(array(
    'id' => $id . '--int_value_unit',
    'val' => $unit_combo_value,
    'options' => $unit_options,
    'width' => 45,
  ));
  if ($value_combo_options_raw) {
    $global_unit_options = array(
      array('val', 'val', 'val'),
      array('%', '%', '%'),
    );
    $fields[] = _style_manager_field_combo(array(
      'id' => $id . '--int_value_global_unit',
      'val' => $global_unit_combo_value,
      'options' => $global_unit_options,
      'width' => 45,
    ));
  }
  return $fields;
}


/**
 * Padding/margin fieldgroup.
 */
function _style_manager_box_indent($variables, $options) {
  $data = $variables['data'];
  $settings = $data['settings'];
  $id = $data['id'];
  $settings['top'] = isset($settings['top']) ? $settings['top'] : 'none';
  $settings['right'] = isset($settings['right']) ? $settings['right'] : 'none';
  $settings['bottom'] = isset($settings['bottom']) ? $settings['bottom'] : 'none';
  $settings['left'] = isset($settings['left']) ? $settings['left'] : 'none';
  $options_t = $options;
  $options_r = $options;
  $options_b = $options;
  $options_l = $options;

  $options_r[] = $options_b[] = $options_l[] = array(
    'copy--top',
    'As Top',
    'As Top',
  );
  $options_t[] = $options_b[] = $options_l[] = array(
    'copy--right',
    'As Right',
    'As Right',
  );
  $options_t[] = $options_r[] = $options_l[] = array(
    'copy--bottom',
    'As Bottom',
    'As Bottom',
  );
  $options_t[] = $options_r[] = $options_b[] = array(
    'copy--left',
    'As Left',
    'As Left',
  );

  $items = array();
  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--top',
      'value' => $settings['top'],
      'options' => $options_t,
    )),
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => t('Right'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 95,
      'margin' => '0',
    ),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--right',
      'value' => $settings['right'],
      'options' => $options_r,
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Top'),
    'items' => $fields,
  ));

  $fields = array(
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--bottom',
      'value' => $settings['bottom'],
      'options' => $options_b,
    )),
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => t('Left'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 95,
      'margin' => '0',
    ),
    _style_manager_jsform_advanced_numeric(array(
      'id' => $id . '--left',
      'value' => $settings['left'],
      'options' => $options_l,
    )),
  );
  $items[] = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Bottom'),
    'items' => $fields,
  ));
  return $items;
}

/**
 * Return tab with color overriding settings.
 *
 * @see style_manager_css_code()
 */
function style_manager_color_overriding_tab($id, $i, $settings) {
  $combo_options = array(
    'id' => $id . '--color' . $i . '--mode',
    'val' => isset($settings['color' . $i]['mode']) ? $settings['color' . $i]['mode'] : '-',
    'options' => style_manager_color_overriding_options(),
    'width' => 390,
  );
  $image_colorize_settings_field = _style_manager_field_combo($combo_options);

  $options = array(
    'settings' => empty($settings['color' . $i]['color']) ? FALSE : $settings['color' . $i]['color'],
    'id' => $id . '_color' . $i,
    'fields_id' => $id . '--color' . $i . '--color--',
    'color_field_title' => t('Replace color'),
    'hide_fieldset' => TRUE,
  );
  $color_form = style_manager_color(array('data' => $options));

  $colorize_container = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Replace mode'),
    'id' => $id . '--color' . $i . '--colorize_container',
    'items' => array($image_colorize_settings_field),
    'margin' => '5px 0 5px 0',
  ));

  $or_color_colorpicker = _style_manager_field_colorpicker(array(
    'val' => $settings['color' . $i]['from_color'],
    'id' => $id . '--color' . $i . '--from_color',
    'hide_prefix' => TRUE,
  ));

  $or_color_hue_tolerance = array(
    'xtype' => 'numberfield',
    'name' => $id . '--color' . $i . '--hue_tolerance',
    'id' => $id . '--color' . $i . '--hue_tolerance',
    'margin' => '0 2px 0 0px',
    'width' => 50,
    'maxValue' => 100,
    'minValue' => 0,
    'step' => 5,
    'allowBlank' => TRUE,
    'value' => isset($settings['color' . $i]['hue_tolerance']) ? $settings['color' . $i]['hue_tolerance'] : (($i == 1) ? 90 : 10),
  );
  $or_color_saturation_tolerance = array(
    'xtype' => 'numberfield',
    'name' => $id . '--color' . $i . '--saturation_tolerance',
    'id' => $id . '--color' . $i . '--saturation_tolerance',
    'margin' => '0 2px 0 0px',
    'width' => 50,
    'maxValue' => 100,
    'minValue' => 0,
    'step' => 5,
    'allowBlank' => TRUE,
    'value' => isset($settings['color' . $i]['saturation_tolerance']) ? $settings['color' . $i]['saturation_tolerance'] : '95',
  );
  $or_color_lightness_tolerance = array(
    'xtype' => 'numberfield',
    'name' => $id . '--color' . $i . '--lightness_tolerance',
    'id' => $id . '--color' . $i . '--lightness_tolerance',
    'margin' => '0 2px 0 0px',
    'width' => 50,
    'maxValue' => 100,
    'minValue' => 0,
    'step' => 5,
    'allowBlank' => TRUE,
    'value' => isset($settings['color' . $i]['lightness_tolerance']) ? $settings['color' . $i]['lightness_tolerance'] : '95',
  );
  $or_color_weight = array(
    'xtype' => 'numberfield',
    'name' => $id . '--color' . $i . '--weight',
    'id' => $id . '--color' . $i . '--weight',
    'margin' => '0',
    'width' => 55,
    'maxValue' => 100,
    'minValue' => -100,
    'step' => 10,
    'allowBlank' => TRUE,
    'value' => isset($settings['color' . $i]['weight']) ? $settings['color' . $i]['weight'] : ($i * -10 + 10),
  );
  $original_color_data = array(
    $or_color_colorpicker,
    array(
      'margin' => '0 3px 0 7px',
      'xtype' => 'displayfield',
      'value' => 'Tolerance (HSL) В±',
    ),
    $or_color_hue_tolerance,
    array(
      'xtype' => 'displayfield',
      'value' => '/',
    ),
    $or_color_saturation_tolerance,
    array(
      'xtype' => 'displayfield',
      'value' => '/',
    ),
    $or_color_lightness_tolerance,
    array(
      'xtype' => 'displayfield',
      'fieldLabel' => t('Weight'),
      'labelAlign' => 'right',
      'hideLabel' => FALSE,
      'labelWidth' => 59,
      'margin' => '0',
    ),
    $or_color_weight,
  );
  $original_color_container = _style_manager_jsform_fieldcontainer(array(
    'title' => t('Find color'),
    'items' => $original_color_data,
    'id' => $id . '--color' . $i . '--original_color_container',
    'margin' => '0 0 5px 0',
  ));

  array_unshift($color_form, $colorize_container);
  array_unshift($color_form, $original_color_container);
  $option_tab = _style_manager_jsform_fieldset(array(
    'title' => '',
    'collapsible' => FALSE,
    'items' => $color_form,
  ));

  $tab = array(
    'id' => $id . '--color' . $i . '--tab',
    'title' => (($i == 1) ? t('Color overriding #') : '#') . $i,
    'padding' => '5px 0 0 0',
    'frame' => TRUE,
    'items' => array($option_tab),
    'disabled' => empty($settings['color_overriding']),
  );

  return $tab;
}

/**
 * Return array with color overriding modes.
 */
function style_manager_color_overriding_options() {
  return array(
    array(
      '-',
      'Disable',
      'Disable',
    ),
    array(
      'mode_h',
      '1 - Hue',
      '1 - Hue',
    ),
    array(
      'mode_hs',
      '2 - Hue/Saturation',
      '2 - Hue/Saturation',
    ),
    array(
      'mode_hl',
      '3 - Hue/Lightness',
      '3 - Hue/Lightness',
    ),
    array(
      'mode_hsl',
      '4 - Hue/Saturation/Lightness',
      '4 - Hue/Saturation/Lightness',
    ),
  );
}
