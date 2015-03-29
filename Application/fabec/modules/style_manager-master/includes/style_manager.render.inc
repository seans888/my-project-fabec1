<?php

/**
 * @file
 * Render css from preset settings.
 */

/**
 * Render presets array to css code or save it to file.
 */
function style_manager_render_styles($presets, $file_path = FALSE) {
  $style = '';
  if ($file_path) {
    variable_set('style_manager_css3pie', array());
  }
  foreach ($presets as $cat => $preset_name) {
    $style .= style_manager_preset_to_css($cat, $preset_name, $file_path);
  }
  if ($file_path) {
    $pathinfo = pathinfo($file_path);
    if (!file_prepare_directory($pathinfo['dirname'], FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
      return FALSE;
    }
    $file_info = t("/**\r\n * Created by style_manager module.\r\n * Version: @version \r\n */\r\n", array('@version' => style_manager_get_version()));
    $style = $file_info . $style;
    return file_save_data($style, $file_path, FILE_EXISTS_REPLACE);
  }
  else {
    return $style;
  }
}


/**
 * Render presets to css code.
 */
function style_manager_preset_to_css($cat, $preset_name, $upd_pie = FALSE) {
  $preset_settings = style_manager_preset_get($cat, $preset_name, TRUE);
  $cat_info = style_manager_get_cat_info($cat);
  style_manager_set_active_category($cat, $preset_name);
  $css3pie_flags = array('background-image', 'shadow', '-pie-background');
  $css3pie = variable_get('style_manager_css3pie', array());
  if (empty($preset_settings['data'])) {
    return '';
  }
  $css_template = '';
  $css_prefix = isset($cat_info['css_prefix']) ? $cat_info['css_prefix'] : '';
  foreach ($preset_settings['data'] as $key => $data) {
    $preset_settings['data'][$key]['css_prefix'] = $css_prefix;
    $group[$key] = $data['group'];
    $selector[$key] = $data['selector'];
    $type[$key] = $data['type'];
    $title[$key] = $data['title'];
    $weight[$key] = empty($data['weight']) ? 0 : $data['weight'];
  }
  array_multisort($weight, SORT_ASC, $group, SORT_ASC, $title, SORT_ASC, $type, SORT_ASC, $selector, SORT_ASC, $preset_settings['data']);
  foreach ($preset_settings['data'] as $data) {
    if (empty($data['type']) || !style_manager_get_item_visibility_status($cat, $preset_name, $data['group'], $data['id'])) {
      continue;
    }
    $fn = style_manager_get_widget_info($data['type'], 'render');
    if (!function_exists($fn)) {
      continue;
    }
    $css_properties = trim(call_user_func_array($fn, array($data)));
    if ($fn == 'style_manager_css_code_render' && $css_properties) {
      $css_template .= $css_properties;
    }
    elseif ($css_properties) {
      $selectors = style_manager_get_css_selectors($css_prefix, $data['selector']);
      $pie_enable = FALSE;
      foreach ($css3pie_flags as $flag) {
        if (stristr($flag, $css_properties) === FALSE) {
          $pie_enable = TRUE;
        }
      }
      if (!$selectors) {
        continue;
      }
      if ($pie_enable) {
        $css3pie = array_merge($css3pie, $selectors);
      }
      $css_template .= implode(', ', $selectors) . " { {$css_properties} }\r\n";
    }
  }
  if ($upd_pie) {
    variable_set('style_manager_css3pie', array_unique($css3pie));
    cache_clear_all('css3pie_css_cache', 'cache');
  }
  return style_manager_css_code_formating($css_template, !empty($cat_info['add_important']), t("Categoty: @cat_title - @preset_title", array(
    '@cat_title' => $cat_info['title'],
    '@preset_title' => $preset_settings['info']['preset_title'],
  )));
}


/**
 * Color elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_color_render($variables) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.color');
  $settings = $variables['settings'];
  $settings['based_on'] = empty($settings['based_on']) ? '-' : $settings['based_on'];
  if ((!empty($settings['hex']) && $settings['based_on'] == 'hex') || (isset($variables['id']) && $variables['id'] == 'main_color')) {
    return '#' . style_manager_color_check_hex($settings['hex']);
  }
  if ($settings['based_on'] == '-') {
    return FALSE;
  }
  else {
    if (empty($settings['based_on'])) {
      $settings['based_on'] = 'main_color';
    }
    $color = style_manager_get_globals_var('global_color', $settings['based_on']);
    if (!empty($settings['color_correct'])) {
      $hsl = style_manager_color_hex2hsl($color);
      if (!empty($settings['color_correct'])) {
        $keys = array('h' => 'hue', 's' => 'saturation', 'l' => 'lightness');
        foreach ($keys as $shot_key => $full_key) {
          if (isset($settings[$full_key][0]) && isset($settings[$full_key][1])) {
            switch ($settings[$full_key][0]) {
              case '=':
                $hsl[$shot_key] = (int) $settings[$full_key][1];
                break;

              case '+':
                $hsl[$shot_key] += (int) $settings[$full_key][1];
                break;

              case '-':
                $hsl[$shot_key] -= (int) $settings[$full_key][1];
                break;

            }
            if ($shot_key == 'h') {
              if ($hsl[$shot_key] < 0) {
                $hsl[$shot_key] += 360;
              }
              if ($hsl[$shot_key] > 359) {
                $hsl[$shot_key] -= 360;
              }
            }
            else {
              if ($hsl[$shot_key] < 0) {
                $hsl[$shot_key] = 0;
              }
              if ($hsl[$shot_key] > 100) {
                $hsl[$shot_key] = 100;
              }
            }
          }
        }
      }
      $color = '#' . style_manager_color_hsl2hex($hsl['h'], $hsl['s'], $hsl['l']);
    }
    return $color;
  }
  return FALSE;
}


/**
 * Css code render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_css_code_render($variables) {
  if (empty($variables['settings'])) {
    return '';
  }
  $settings = $variables['settings'];
  $out = $settings['css_code'];
  if ($settings['color_overriding'] && trim($settings['css_code'])) {
    $color_overriding = _style_manager_overriding_colors_data($settings);
    if ($color_overriding) {
      $out = _style_manager_css_color_overriding($color_overriding, $out);
      if ($settings['images_overriding']) {
        $out = _style_manager_css_images_overriding($color_overriding, $out);
      }
    }
  }

  if ($variables['css_prefix'] && $settings['add_css_prefix']) {
    _style_manager_css_selector_formating_preg_callback(NULL, array('css_prefix' => $variables['css_prefix']));
    $out = preg_replace_callback('/([^}{]*){/isU', '_style_manager_css_selector_formating_preg_callback', $out);

  }

  return $out;
}


/**
 * Gradient elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_gradient_render($variables) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.color');
  if (empty($variables['settings'])
      || empty($variables['settings']['preset'])
      || $variables['settings']['preset'] == '-') {
    return '';
  }
  $settings = $variables['settings'];
  $css_code = preg_replace('/\/\*.*\*\//isU', '', $settings['css_code']);
  $main_color = style_manager_color_render(array('settings' => $settings['color']));

  if ($main_color && $settings['color_mode'] != '-') {
    $color_overriding = array();
    $color_overriding[] = array(
      'from_color' => array(
        'rgb' => FALSE,
        'hsl' => FALSE,
      ),
      'to_color' => array(
        'rgb' => style_manager_color_hex2rgb($main_color),
        'hsl' => style_manager_color_hex2hsl($main_color),
      ),
      'mode' => $settings['color_mode'],
      'hue_tolerance' => 360,
      'lightness_tolerance' => 100,
      'saturation_tolerance' => 100,
      'weight' => 50,
    );
    $css_code = _style_manager_css_color_overriding($color_overriding, $css_code);
  }
  return style_manager_gradient_add_rules($css_code);
}


/**
 * Background elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_background_render($variables) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.image');
  if (empty($variables['settings'])) {
    return '';
  }
  $out = '';
  $settings = $variables['settings'];

  $bg_color = style_manager_color_render(array('settings' => $settings['color']));
  if (!empty($settings['color']['transparent'])) {
    $out .= "background-color: transparent; ";
  }
  elseif ($bg_color) {
    $out .= "background-color: $bg_color; ";
  }
  if (!empty($settings['image']['url']) && $settings['image']['url'] == 'none') {
    $out .= "background-image: none; ";
  }
  elseif (!empty($settings['image']['url']) && $settings['image']['url'] != '-') {
    $url = _style_manager_background_render_image_processing($settings);
    $out .= "background-image: url('{$url}'); ";
  }

  if (!empty($settings['image']['repeat']) && $settings['image']['repeat'] != '-') {
    $out .= "background-repeat: {$settings['image']['repeat']}; ";
  }
  if (!empty($settings['image']['attachment']) && $settings['image']['attachment'] != '-') {
    $out .= "background-attachment: {$settings['image']['attachment']}; ";
  }
  if (!empty($settings['image']['position-x']) && $settings['image']['position-x'] != '-' && !empty($settings['image']['position-y']) && $settings['image']['position-y'] != '-') {
    $position_y = style_manager_jsform_advanced_numeric_render($settings['image']['position-y']);
    $position_x = style_manager_jsform_advanced_numeric_render($settings['image']['position-x']);
    if ($position_x !== FALSE) {
      $out .= "background-position: {$position_x} {$position_y}; ";
    }
  }
  return $out;
}


/**
 * Padding elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_padding_render($variables) {
  return style_manager_box_indent_render($variables, 'padding');
}


/**
 * Margin elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_margin_render($variables) {
  return style_manager_box_indent_render($variables, 'margin');
}


/**
 * List style render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_list_style_render($variables) {
  $out = '';
  foreach ($variables['settings'] as $key => $data) {
    if (empty($data) || $data == '-') {
      continue;
    }
    else {
      if ($key == 'image' && $data != 'none') {
        $data = '"' . $data . '"';
      }
      $out .= " list-style-{$key}: {$data}; ";
    }
  }
  return $out;
}


/**
 * Reset render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_reset_render($variables) {
  $out = '';
  $settings = $variables['settings'];

  if ($settings['type'] == 'all' || !empty($settings['custom']['background'])) {
    $out .= ' background-color: transparent; ';
    $out .= ' background-image: none; ';
  }
  if ($settings['type'] == 'all' || !empty($settings['custom']['border'])) {
    $out .= ' border: 0; ';
  }
  if ($settings['type'] == 'all' || !empty($settings['custom']['font'])) {
    $out .= ' font-size: 100%; ';
    $out .= ' font: inherit; ';
    $out .= ' text-decoration: inherit; ';
    $out .= ' font-weight: normal; ';
  }
  if ($settings['type'] == 'all' || !empty($settings['custom']['list_style'])) {
    $out .= ' list-style: none; ';
  }
  if ($settings['type'] == 'all' || !empty($settings['custom']['margin'])) {
    $out .= ' margin: 0; ';
  }
  if ($settings['type'] == 'all' || !empty($settings['custom']['padding'])) {
    $out .= ' padding: 0; ';
  }
  if ($settings['type'] == 'all' || !empty($settings['custom']['position'])) {
    $out .= ' position: static; ';
  }
  return $out;
}


/**
 * Padding/margin render.
 *
 * @see style_manager_margin_render()
 */
function style_manager_box_indent_render($variables, $type) {
  $out = '';
  $el_arr = array('top', 'right', 'bottom', 'left');
  foreach ($el_arr as $el_id) {
    if (!empty($variables['settings'][$el_id])) {
      $val = style_manager_jsform_advanced_numeric_render($variables['settings'][$el_id], $variables['settings']);
      if ($val !== FALSE && $val !== '-') {
        $out .= " {$type}-{$el_id}: {$val}; ";
      }
    }
  }
  return $out;
}


/**
 * Box-shadow elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_box_shadow_render($variables) {
  $settings = $variables['settings'];
  $property_arr = array('-moz-box-shadow', '-webkit-box-shadow', 'box-shadow');
  $out = '';
  if (!empty($settings['disable'])) {
    foreach ($property_arr as $property) {
      $out .= " {$property}: none; ";
    }
    return $out;
  }
  module_load_include('inc', 'style_manager', 'includes/style_manager.color');
  $hex_color = style_manager_color_render(array('settings' => $settings['color']));
  $offset_x = style_manager_jsform_advanced_numeric_render($settings['offset_x']);
  $offset_y = style_manager_jsform_advanced_numeric_render($settings['offset_y']);
  $size = isset($settings['size']) ? style_manager_jsform_advanced_numeric_render($settings['size']) : '';
  $blur = isset($settings['blur']) ? style_manager_jsform_advanced_numeric_render($settings['blur']) : '';
  $inset = empty($settings['inset']) ? '' : 'inset ';

  if ($offset_y === FALSE || $offset_x === FALSE || $hex_color === FALSE) {
    return '';
  }
  foreach ($property_arr as $property) {
    $out .= " {$property}: {$inset} {$offset_x} {$offset_y} {$blur} {$size} {$hex_color}; ";
  }
  return $out;
}


/**
 * Position elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_position_render($variables) {
  $settings = $variables['settings'];
  $out = '';

  if (!empty($settings['position']) && $settings['position'] != '-') {
    $out .= " position: {$settings['position']}; ";
  }
  if (isset($settings['z_index']) && $settings['z_index'] != '') {
    $out .= " z-index: {$settings['z_index']}; ";
  }
  $placement = array();
  $placement['top'] = style_manager_jsform_advanced_numeric_render($settings['placement']['top']);
  $placement['right'] = style_manager_jsform_advanced_numeric_render($settings['placement']['right']);
  $placement['bottom'] = style_manager_jsform_advanced_numeric_render($settings['placement']['bottom']);
  $placement['left'] = style_manager_jsform_advanced_numeric_render($settings['placement']['left']);
  foreach ($placement as $key => $data) {
    if ($data !== FALSE && $data !== '') {
      $out .= " {$key}: {$data}; ";
    }
  }
  return $out;
}


/**
 * Box elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_box_render($variables) {
  $settings = $variables['settings'];
  $out = '';

  if (!empty($settings['display']) && $settings['display'] != '-') {
    $out .= " display: {$settings['display']}; ";
  }
  if (!empty($settings['clear']) && $settings['clear'] != '-') {
    $out .= " clear: {$settings['clear']}; ";
  }
  if (!empty($settings['float']) && $settings['float'] != '-') {
    $out .= " float: {$settings['float']}; ";
  }
  if (!empty($settings['overflow']) && $settings['overflow'] != '-') {
    $out .= " overflow: {$settings['overflow']}; ";
  }
  $size = array();
  $size['height'] = style_manager_jsform_advanced_numeric_render($settings['height']);
  $size['width'] = style_manager_jsform_advanced_numeric_render($settings['width']);
  foreach ($size as $key => $data) {
    if ($data !== NULL && $data !== '-' && trim($data) !== '') {
      $out .= " {$key}: {$data}; ";
    }
  }
  return $out;
}


/**
 * Border elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_border_render($variables) {
  $settings = $variables['settings'];
  $out = '';

  if (!empty($settings['style']) && !empty($settings['width']) && !empty($settings['color'])) {
    $color = style_manager_color_render(array('settings' => $settings['color']['default']));
    $width = style_manager_jsform_advanced_numeric_render($settings['width']);
    $borders = array('top', 'right', 'bottom', 'left');
    foreach ($borders as $data) {
      if (empty($settings[$data])) {
        continue;
      }
      if ($width !== FALSE && (!isset($settings['style']) || $settings['style'] != 'none')) {
        $out .= " border-{$data}-width: {$width}; ";
      }
      if ($settings['style'] && $settings['style'] != '-') {
        $out .= " border-{$data}-style: {$settings['style']}; ";
      }
      if (!empty($settings['different_color']) && (!isset($settings['style']) || $settings['style'] != 'none')) {
        $color = style_manager_color_render(array('settings' => $settings['color'][$data]));
      }
      if ($color !== FALSE && (!isset($settings['style']) || $settings['style'] != 'none')) {
        $out .= " border-{$data}-color: {$color}; ";
      }
    }
  }
  if (!empty($settings['border_radius'])) {
    $el_arr = array('top_left', 'top_right', 'bottom_right', 'bottom_left');
    $border_radius_arr = array();
    $add_border_radius = FALSE;
    foreach ($el_arr as $key) {
      if (!empty($settings['border_radius'][$key])) {
        $val = style_manager_jsform_advanced_numeric_render($settings['border_radius'][$key], $settings['border_radius']);
        if ($val !== FALSE && $val !== '-') {
          $border_radius_arr[] = $val;
          $add_border_radius = TRUE;
        }
        else {
          $border_radius_arr[] = 0;
        }
      }
    }
    if ($add_border_radius) {
      $border_radius_property = implode(' ', $border_radius_arr);
      $out .= " -moz-border-radius: {$border_radius_property}; ";
      $out .= " -webkit-border-radius: {$border_radius_property}; ";
      $out .= " border-radius: {$border_radius_property}; ";
    }
  }
  return $out;
}


/**
 * Font elements render.
 *
 * @see style_manager_style_manager_widgets()
 */
function style_manager_font_render($variables) {
  $settings = $variables['settings'];
  $out = '';

  if (isset($settings['font_family']) && $settings['font_family'] != '-') {
    $font = style_manager_get_globals_var('global_font', $settings['font_family']);
    $out .= " font-family: {$font}  ; ";
  }
  if (isset($settings['font_weight']) && $settings['font_weight'] != '-') {
    $out .= " font-weight: {$settings['font_weight']}; ";
  }
  if (isset($settings['font_style']) && $settings['font_style'] != '-') {
    $out .= " font-style: {$settings['font_style']}; ";
  }
  if (isset($settings['text_transform']) && $settings['text_transform'] != '-') {
    $out .= " text-transform: {$settings['text_transform']}; ";
  }
  if (isset($settings['text_decoration']) && $settings['text_decoration'] != '-') {
    $out .= " text-decoration: {$settings['text_decoration']}; ";
  }
  if (isset($settings['text_align']) && $settings['text_align'] != '-') {
    $out .= " text-align: {$settings['text_align']}; ";
  }
  if (isset($settings['line_height'])) {
    $line_height = style_manager_jsform_advanced_numeric_render($settings['line_height']);
    if ($line_height !== FALSE) {
      $out .= " line-height: {$line_height}; ";
    }
  }
  if (isset($settings['font_size'])) {
    $font_size = style_manager_jsform_advanced_numeric_render($settings['font_size']);
    if ($font_size !== FALSE) {
      $out .= " font-size: {$font_size}; ";
    }
  }
  if (isset($settings['color'])) {
    $color = style_manager_color_render(array('settings' => $settings['color']));
    $out .= $color ? " color: {$color}; " : "";
  }
  if (isset($settings['shadow']['color']) && isset($settings['shadow']['offset_x']) && isset($settings['shadow']['offset_y'])) {
    $shadow_color = style_manager_color_render(array('settings' => $settings['shadow']['color']));
    $offset_x = style_manager_jsform_advanced_numeric_render($settings['shadow']['offset_x']);
    $offset_y = style_manager_jsform_advanced_numeric_render($settings['shadow']['offset_y']);
    $blur = style_manager_jsform_advanced_numeric_render($settings['shadow']['blur']);
    if ($offset_x !== FALSE && $offset_y !== FALSE && $shadow_color !== FALSE) {
      $out .= " text-shadow: {$offset_x} {$offset_y} {$blur} {$shadow_color}; ";
    }
  }
  return $out;
}


/**
 * Advanced numeric fields render.
 */
function style_manager_jsform_advanced_numeric_render($value, $all_vars = array(), $max_depth = 20) {
  if ($max_depth == 0) {
    return FALSE;
  }
  if (is_string($value) || is_numeric($value)) {
    if ($value == '') {
      return FALSE;
    }
    else {
      return $value;
    }
  }
  elseif (isset($value['int_value']) && isset($value['int_value_unit']) && is_numeric($value['int_value']) && ((isset($value['str_value']) && $value['str_value'] == 'value') || !isset($value['str_value']))
  ) {
    return $value['int_value'] . $value['int_value_unit'];
  }
  elseif (isset($value['str_value']) && !in_array($value['str_value'], array('value', '-'))) {
    if (strstr($value['str_value'], 'global--')) {
      $global = style_manager_get_globals_var('global_numeric', str_replace('global--', '', $value['str_value']));
      $global_val = style_manager_jsform_advanced_numeric_render($global);
      preg_match_all('/([\1-9]+?)([A-Za-z\%]+?)/isU', $global_val, $global_val_arr, PREG_SET_ORDER);
      if (empty($global_val_arr[0][2])) {
        return FALSE;
      }
      $global_int_val = $global_val_arr[0][1];
      $global_unit = $global_val_arr[0][2];
      if (!empty($value['int_value'])) {
        if (isset($value['int_value_global_unit']) && $value['int_value_global_unit'] == '%') {
          $global_int_val = (int) (($global_int_val / 100) * $value['int_value']);
        }
        else {
          $global_int_val += $value['int_value'];
        }
      }
      return $global_int_val . $global_unit;
    }
    elseif (strstr($value['str_value'], 'copy--')) {
      $key = str_replace('copy--', '', $value['str_value']);
      if (empty($all_vars[$key])) {
        return FALSE;
      }
      else {
        $max_depth--;
        return style_manager_jsform_advanced_numeric_render($all_vars[$key], $all_vars, $max_depth);
      }
    }
    else {
      return $value['str_value'];
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Add missing rules for gradient.
 */
function style_manager_gradient_add_rules($css) {
  if (stristr($css, 'radial') === FALSE) {
    $type = 'linear';
  }
  else {
    $type = 'radial';
  }
  $css = str_replace(array('linear', 'radial'), '{type}', $css);
  preg_match_all('/background.*([\{\}\-a-zA-z]*\-gradient)(.*);/Ui', $css, $rules_arr, PREG_SET_ORDER);
  $types = array(
    '-webkit-gradient' => 'webkit_old',
    '-moz-{type}-gradient' => 'main',
    '-webkit-{type}-gradient' => 'main',
    '-o-{type}-gradient' => 'main',
    '-ms-{type}-gradient' => 'main',
    '{type}-gradient' => 'w3c',
  );
  $rules_by_type = array();
  $rules_by_prefix = array();
  foreach ($rules_arr as $rule) {
    $rules_by_type[$types[$rule[1]]] = $rule[2];
    $rules_by_prefix[$rule[1]] = $rule[2];
  }
  if (empty($rules_by_type['main'])) {
    if (!empty($rules_by_type['w3c'])) {
      $rules_by_type['main'] = _style_manager_gradient_converter($rules_by_type['w3c'], 'w3c_to_main');
    }
    elseif (!empty($rules_by_type['webkit_old']) && $type != 'radial') {
      $rules_by_type['main'] = _style_manager_gradient_converter($rules_by_type['webkit_old'], 'webkit_old_to_main');
    }
  }
  if (empty($rules_by_type['webkit_old']) && $type != 'radial') {
    if (!empty($rules_by_type['w3c'])) {
      $rules_by_type['webkit_old'] = _style_manager_gradient_converter($rules_by_type['w3c'], 'w3c_to_webkit_old');
    }
    elseif (!empty($rules_by_type['main'])) {
      $rules_by_type['webkit_old'] = _style_manager_gradient_converter($rules_by_type['main'], 'main_to_webkit_old');
    }
  }
  if (empty($rules_by_type['w3c'])) {
    if (!empty($rules_by_type['main'])) {
      $rules_by_type['w3c'] = _style_manager_gradient_converter($rules_by_type['main'], 'main_to_w3c');
    }
  }
  foreach ($types as $key => $val) {
    if (empty($rules_by_prefix[$key]) && !empty($rules_by_type[$val])) {
      $css .= " background: {$key}{$rules_by_type[$val]}; ";
    }
  }
  $css = str_replace('{type}', $type, $css);
  return $css;
}

/**
 * Cross-browser convert css gradient.
 */
function _style_manager_gradient_converter($rule, $type) {
  $out = '';
  $main_arr = array(
    'left',
    'top',
    '-45deg',
    '45deg',
  );
  $w3c_arr = array(
    'to right',
    'to bottom',
    '135deg',
    '45deg',
  );
  $webkit_old_arr = array(
    '{type}, left top, right top',
    '{type}, left top, left bottom',
    '{type}, left top, right bottom',
    '{type}, left bottom, right top',
  );

  switch ($type) {
    case 'w3c_to_main':
      $rule = str_replace('(rgb', '(to bottom, ', $rule);
      $out = str_replace($w3c_arr, $main_arr, $rule);
      break;

    case 'webkit_old_to_main':
      preg_match_all('/color-stop\((.*)\s*,\s*([rgbahsl]*\(.*\)|#.*)\)/Ui', $rule, $preg_data, PREG_SET_ORDER);
      if ($preg_data) {
        foreach ($preg_data as $gradient_item) {
          $gradient_item[1] = trim($gradient_item[1]);
          if (is_numeric($gradient_item[1])) {
            $gradient_item[1] *= 100;
            $gradient_item[1] .= '%';
          }
          $rule = str_replace($gradient_item[0], "{$gradient_item[2]} {$gradient_item[1]}", $rule);
        }
        foreach ($webkit_old_arr as $key => $pattern) {
          $pattern = str_replace(' ', '\s*', $pattern);
          $rule = preg_replace("/{$pattern}/Ui", $main_arr[$key], $rule);
        }
        $out = $rule;
      }
      break;

    case 'w3c_to_webkit_old':
      $rule = str_replace('(rgb', '(to bottom, ', $rule);
    case 'main_to_webkit_old':
      preg_match_all('/,\s*([rgbahsl]*\(.*\)|#.*)\s+(.*%)/Ui', $rule, $preg_data, PREG_SET_ORDER);
      if ($preg_data) {
        if ($type == 'w3c_to_webkit_old') {
          $rule = str_replace($w3c_arr, $webkit_old_arr, $rule);
        }
        if ($type == 'main_to_webkit_old') {
          $rule = str_replace($main_arr, $webkit_old_arr, $rule);
        }
        foreach ($preg_data as $gradient_item) {
          $rule = str_replace($gradient_item[0], ", color-stop($gradient_item[2], $gradient_item[1])", $rule);
        }
        $out = $rule;
        if (stristr($rule, '{type}') !== FALSE) {
          $out = $rule;
        }
      }
      break;

    case 'main_to_w3c':
      $out = str_replace($main_arr, $w3c_arr, $rule);
      break;

  }

  return $out;
}

/**
 * Overriding css color.
 */
function _style_manager_css_color_overriding($color_overriding, $css_code) {
  _style_manager_color_overridden_preg_callback(NULL, array('color_overriding' => $color_overriding));
  $css_code = preg_replace_callback('/(#)([a-f0-9]{6})|(#)([a-f0-9]{3})|(rgba)\((.*)\)|(rgb)\((.*)\)/Ui', '_style_manager_color_overridden_preg_callback', $css_code);
  return $css_code;
}

/**
 * Overriding images.
 */
function _style_manager_css_images_overriding($color_overriding, $css_code) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.image');
  _style_manager_images_overridden_preg_callback(NULL, array('color_overriding' => $color_overriding));
  $css_code = preg_replace_callback('/url\(\s*[\'"]?([^\'"\)]*)[\'"]?\s*\)/i', '_style_manager_images_overridden_preg_callback', $css_code);
  return $css_code;
}


/**
 * Overriding images.
 * @see: _style_manager_css_images_overriding
 */
function _style_manager_images_overridden_preg_callback($matches, $settings = NULL) {
  $_settings = & drupal_static(__FUNCTION__);
  if (isset($settings)) {
    $_settings = array(
      'data' => $settings,
      'cache' => array(),
    );
    return FALSE;
  }

  $image_overridden_settings = array(
    'url' => $matches[1],
    'color_overriding' => $_settings['data']['color_overriding'],
    'transparent' => FALSE,
    'lightness' => FALSE,
    'contrast' => FALSE,
    'brightness' => FALSE,
    'transparency' => FALSE,
    'background_color' => FALSE,
  );
  $url = style_manager_image_post_processing($image_overridden_settings);
  if ($url) {
    return "url('{$url}')";
  }
  return $matches[0];
}


/**
 * Overriding color.
 * @see: _style_manager_css_color_overriding
 */
function _style_manager_color_overridden_preg_callback($matches, $settings = NULL) {
  $_settings = & drupal_static(__FUNCTION__);
  if (isset($settings)) {
    $_settings = array(
      'data' => $settings,
      'cache' => array(),
    );
    return FALSE;
  }
  if (isset($_settings['cache'][$matches[0]])) {
    return $_settings['cache'][$matches[0]];
  }
  $color_data = array(array_pop($matches), array_pop($matches));
  if ($color_data[1] == '#') {
    $hsl = style_manager_color_hex2hsl(style_manager_color_check_hex($matches[0]));
  }
  else {
    $rgb_arr = explode(',', $color_data[0]);
    $hsl = style_manager_color_rgb2hsl(trim($rgb_arr[0]), trim($rgb_arr[1]), trim($rgb_arr[2]));
  }
  $overridden_settings = FALSE;
  if (!empty($_settings['data']['color_overriding'])) {
    $overridden_settings = _style_manager_get_overriding_settings_for_color($_settings['data']['color_overriding'], $hsl);
    if (!$overridden_settings) {
      $_settings['cache'][$matches[0]] = $matches[0];
      return $matches[0];
    }
  }
  $hsl = style_manager_color_convert($overridden_settings['to_color']['hsl'], $hsl, $overridden_settings['mode']);
  if ($color_data[1] == '#') {
    $replacement_val = '#' . style_manager_color_hsl2hex($hsl['h'], $hsl['s'], $hsl['l']);
  }
  else {
    $result_rgb = style_manager_color_hsl2rgb($hsl['h'], $hsl['s'], $hsl['l']);
    $rgb_arr[0] = $result_rgb['r'];
    $rgb_arr[1] = $result_rgb['g'];
    $rgb_arr[2] = $result_rgb['b'];
    $replacement_val = $color_data[1] . '(' . implode(', ', $rgb_arr) . ')';
  }

  $_settings['cache'][$matches[0]] = $replacement_val;
  return $replacement_val;
}


/**
 * Return overriding settings for color.
 */
function _style_manager_get_overriding_settings_for_color($color_overridden_data, $hsl_color) {
  $overridden_settings = FALSE;
  foreach ($color_overridden_data as $data) {
    if ($data['from_color']['hsl'] == FALSE
        || (
          abs($data['from_color']['hsl']['h'] - $hsl_color['h']) <= $data['hue_tolerance']
          || abs($data['from_color']['hsl']['h'] - $hsl_color['h'] - 360) <= $data['hue_tolerance']
        )
        && abs($data['from_color']['hsl']['l'] - $hsl_color['l']) <= $data['lightness_tolerance']
        && abs($data['from_color']['hsl']['s'] - $hsl_color['s']) <= $data['saturation_tolerance']
    ) {
      $overridden_settings = $data;
      break;
    }
  }
  return $overridden_settings;
}

/**
 * Preparing setting for image post processing.
 * @see: style_manager_background_render()
 */
function _style_manager_background_render_image_processing($settings) {
  $post_processing_settings = $settings['image_post_processing'];
  $url = $settings['image']['url'];
  if (!empty($settings['color_overriding'])) {
    $background_color = style_manager_color_render(array('settings' => $settings['color']));
    $image_overridden_settings = array(
      'url' => $url,
      'color_overriding' => _style_manager_overriding_colors_data($settings),
      'transparent' => $settings['color']['transparent'],
      'lightness' => $post_processing_settings['lightness'],
      'contrast' => $post_processing_settings['contrast'],
      'brightness' => $post_processing_settings['brightness'],
      'transparency' => $post_processing_settings['transparency'],
      'background_color' => FALSE,
    );
    if ($background_color) {
      $image_overridden_settings['background_color'] = array(
        'rgb' => style_manager_color_hex2rgb($background_color),
        'hsl' => style_manager_color_hex2hsl($background_color),
      );
    }
    $url = style_manager_image_post_processing($image_overridden_settings);
  }
  return $url;
}

/**
 * Preparing overriding colors data.
 * @see: style_manager_background_render()
 */
function _style_manager_overriding_colors_data($settings) {
  $color_overriding = array();
  for ($i = 1; $i < 20; $i++) {
    if (!empty($settings['color' . $i])) {
      $color_settings = $settings['color' . $i];
      if ($color_settings['mode'] == '-') {
        continue;
      }
      $to_color = style_manager_color_render(array('settings' => $color_settings['color']));
      if (!$to_color) {
        continue;
      }
      $from_color = style_manager_color_check_hex($color_settings['from_color']);
      $color_overriding[] = array(
        'from_color' => array(
          'rgb' => $from_color ? style_manager_color_hex2rgb($from_color) : FALSE,
          'hsl' => $from_color ? style_manager_color_hex2hsl($from_color) : FALSE,
        ),
        'to_color' => array(
          'rgb' => style_manager_color_hex2rgb($to_color),
          'hsl' => style_manager_color_hex2hsl($to_color),
        ),
        'mode' => $color_settings['mode'],
        'hue_tolerance' => (int) ($color_settings['hue_tolerance'] * 3.6),
        'saturation_tolerance' => $color_settings['saturation_tolerance'],
        'lightness_tolerance' => $color_settings['lightness_tolerance'],
        'weight' => $color_settings['weight'],
      );
    }
  }
  if ($color_overriding) {
    foreach ($color_overriding as $key => $data) {
      $weight[$key] = $data['weight'];
    }
    array_multisort($weight, SORT_ASC, $color_overriding);
  }
  return $color_overriding;
}
