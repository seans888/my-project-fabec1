<?php

/**
 * @file
 * Helper functions.
 */

/**
 * Return array with all category's.
 */
function style_manager_get_cat_list() {
  $cache = cache_get('style_manager_cat_list');
  if (!empty($cache->data)) {
    return $cache->data;
  }
  $out = array();
  $result = db_select('variable', 'v')
    ->condition('v.name', 'style_manager_preset%', 'LIKE')
    ->fields('v', array('name'))->execute()->fetchAll();

  if ($result) {
    foreach ($result as $data) {
      $name = str_replace('style_manager_preset_', '', $data->name);
      $out[$name] = $name;
    }
  }
  cache_set('style_manager_cat_list', $out);
  return $out;
}


/**
 * Return category info.
 */
function style_manager_get_cat_info($cat_id = FALSE, $var = FALSE, $type = 'info') {
  if ($cat_id) {
    $cat_data = variable_get('style_manager_preset_' . $cat_id, array());
    $cat_data = style_manager_get_cat_info_add_id($cat_data);
  }
  if (!$cat_id) {
    $all_cat_data = array();
    $cat_list = style_manager_get_cat_list();
    foreach ($cat_list as $cat) {
      $cat_data = variable_get('style_manager_preset_' . $cat, array());
      $cat_data = style_manager_get_cat_info_add_id($cat_data);
      $all_cat_data[$cat_data['info']['id']] = $cat_data;
    }
    return $all_cat_data;
  }
  if ($var) {
    if (!empty($cat_data[$type][$var])) {
      return $cat_data[$type][$var];
    }
    else {
      return FALSE;
    }
  }
  elseif (!empty($cat_data[$type]) && !$var) {
    return $cat_data[$type];
  }
  else {
    return FALSE;
  }
}

/**
 * Add id key for all elements in array.
 */
function style_manager_get_cat_info_add_id($cat_data) {
  if (!empty($cat_data['data'])) {
    foreach ($cat_data['data'] as $key => $data) {
      $cat_data['data'][$key]['id'] = $key;
    }
  }
  return $cat_data;
}

/**
 * Set category info.
 */
function style_manager_set_cat_info($cat_id, $data, $type = 'info') {
  $store_data = variable_get('style_manager_preset_' . $cat_id, array());
  $store_data[$type] = $data;
  variable_set('style_manager_preset_' . $cat_id, $store_data);
}


/**
 * Set default preset.
 */
function style_manager_set_default_preset($cat_id, $preset_id, $tmp = FALSE) {
  $default_preset = & drupal_static('style_manager_default_preset');
  if (empty($default_preset)) {
    $default_preset = array();
  }
  $default_preset[$cat_id] = $preset_id;
  if (!$tmp) {
    $cat_info = style_manager_get_cat_info($cat_id);
    $cat_info['default_preset'] = $preset_id;
    style_manager_set_cat_info($cat_id, $cat_info);
  }
}


/**
 * Get default preset.
 */
function style_manager_get_default_preset($cat_id) {
  $default_preset_arr = & drupal_static('style_manager_default_preset');
  if (!empty($default_preset_arr[$cat_id])) {
    return $default_preset_arr[$cat_id];
  }

  $default_preset = style_manager_get_cat_info($cat_id, 'default_preset');
  if (!$default_preset) {
    $default_preset = 'default';
  }
  $default_preset_arr[$cat_id] = $default_preset;
  return $default_preset;
}


/**
 * Get active preset for current page.
 */
function style_manager_get_page_preset($cat_id) {
  $default_preset = style_manager_get_cat_info($cat_id, 'default_preset');
  if (!$default_preset) {
    $default_preset = 'default';
  }

  if (module_exists('context')) {
    $contexts = context_active_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions['style_manager_reaction_preset'][$cat_id])) {
        $default_preset = $context->reactions['style_manager_reaction_preset'][$cat_id];
        style_manager_set_default_preset($cat_id, $default_preset, TRUE);
      }
    }
  }
  return $default_preset;
}


/**
 * Check is default preset overridden in context module.
 */
function style_manager_preset_is_override($cat_id, $preset) {
  $out = FALSE;
  if (module_exists('context')) {
    $context_list = context_load(NULL, TRUE);
    $act_contexts = array();
    foreach ($context_list as $context) {
      if (!empty($context->reactions['style_manager_reaction_preset'][$cat_id]) && $context->reactions['style_manager_reaction_preset'][$cat_id] == $preset) {
        $act_contexts[] = $context->name;
      }
    }
    if ($act_contexts) {
      $out = t('Set as default in context: @context_name', array('@context_name' => implode(', ', $act_contexts)));
    }
  }
  return $out;
}

/**
 * Get all presets.
 */
function style_manager_get_all_presets($cat) {
  $store_data = variable_get('style_manager_preset_' . $cat, array());
  if (!empty($store_data['preset_data'])) {
    return $store_data['preset_data'];
  }
  else {
    return array();
  }
}


/**
 * Get preset hash.
 */
function style_manager_get_preset_key($cat, $preset) {
  $preset_data = style_manager_preset_get($cat, $preset);
  if (!empty($preset_data['info']['key'])) {
    return $preset_data['info']['key'];
  }
  elseif ($preset_data) {
    style_manager_preset_set($cat, $preset, $preset_data);
    return style_manager_get_preset_key($cat, $preset);
  }
}


/**
 * Get preset data.
 */
function style_manager_preset_get($cat, $preset, $add_info = FALSE) {
  $presets = style_manager_get_all_presets($cat);
  if (!empty($presets[$preset])) {
    $cat_data = style_manager_get_cat_info($cat, FALSE, 'data');
    foreach ($presets[$preset]['data'] as $key => $data) {
      if (!empty($cat_data[$key])) {
        if ($add_info) {
          $presets[$preset]['data'][$key] = $data + $cat_data[$key];
        }
      }
      else {
        unset($presets[$preset]['data'][$key]);
      }
    }
    return $presets[$preset];
  }
  else {
    return FALSE;
  }
}


/**
 * Get visibility status.
 */
function style_manager_get_item_visibility_status($cat, $preset, $group, $item_id) {
  $preset = style_manager_preset_get($cat, $preset);
  if (empty($preset['info']['visibility_settings'][$group]['visibility_type']) || $preset['info']['visibility_settings'][$group]['visibility_type'] == 'show_all'
  ) {
    return TRUE;
  }
  elseif ($preset['info']['visibility_settings'][$group]['visibility_type'] == 'show_selected') {
    return !empty($preset['info']['visibility_settings'][$group][$item_id]);
  }
  elseif ($preset['info']['visibility_settings'][$group]['visibility_type'] == 'hide_selected') {
    return empty($preset['info']['visibility_settings'][$group][$item_id]);
  }
}


/**
 * Set preset data.
 */
function style_manager_preset_set($cat, $preset, $data) {
  $presets = variable_get('style_manager_preset_' . $cat, array());
  $presets['preset_data'][$preset] = $data;
  $presets['preset_data'][$preset]['info']['key'] = md5(serialize($data['data']));
  variable_set('style_manager_preset_' . $cat, $presets);
}


/**
 * Delete preset.
 */
function style_manager_preset_delete($cat, $preset) {
  $presets = variable_get('style_manager_preset_' . $cat, array());
  unset($presets['preset_data'][$preset]);
  variable_set('style_manager_preset_' . $cat, $presets);
}

/**
 * Delete category.
 */
function style_manager_cat_delete($cat_id) {
  variable_del('style_manager_preset_' . $cat_id);
  cache_set('style_manager_cat_list', FALSE);
}

/**
 * Print data in json.
 */
function style_manager_print_json($data) {
  print json_encode($data);
}


/**
 * Convert url to internal format.
 */
function style_manager_get_rial_url($url) {
  if (file_exists($url)) {
    return $url;
  }
  $tmp_url = strstr($url, 'sites/');
  if (file_exists($tmp_url)) {
    return $tmp_url;
  }
  $tmp_url = strstr($url, 'themes/');
  if (file_exists($tmp_url)) {
    return $tmp_url;
  }
  $tmp_url = substr($url, 0, 1) == '/' ? substr($url, 1) : $url;
  if (file_exists($tmp_url)) {
    return $tmp_url;
  }

  return $url;
}


/**
 * Return array of all categories.
 */
function _style_manager_get_cat_tree() {
  $cat_info = style_manager_get_cat_info();
  $cat_list = array();
  if (!empty($cat_info)) {
    foreach ($cat_info as $key => $val) {
      $cat_list[$key] = $val['info']['title'];
    }
  }
  return $cat_list;
}

/**
 * Return global variable.
 *
 * @see style_manager_style_manager_widgets()
 * @see style_manager_global_color()
 */
function style_manager_get_globals_var($type = 'global_color', $key = FALSE, $cat_id = FALSE, $preset_id = FALSE) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.render');
  $default_preset = style_manager_get_default_preset('global_config');
  $preset = style_manager_preset_get('global_config', $default_preset, TRUE);
  if (empty($preset['data'])) {
    return FALSE;
  }
  $selected_preset = style_manager_get_active_preset();
  if ($selected_preset) {
    $preset['data'] += $selected_preset['data'];
  }

  $global_vars = array();
  foreach ($preset['data'] as $id => $data) {
    if ($id == $key || !$key) {
      if ($data['type'] == 'style_manager_global_color') {
        $global_vars['global_color'][$id]['title'] = $data['title'];
        $global_vars['global_color'][$id]['value'] = style_manager_color_render($data);
      }
      if ($data['type'] == 'style_manager_global_font') {
        $global_vars['global_font'][$id]['title'] = $data['title'];
        $global_vars['global_font'][$id]['value'] = $data['settings']['font'];
      }
      if ($data['type'] == 'style_manager_global_numeric') {
        $global_vars['global_numeric'][$id]['title'] = $data['title'];
        $global_vars['global_numeric'][$id]['value'] = $data['settings']['value'];
      }
    }
  }
  if ($key && !empty($global_vars[$type][$key]['value'])) {
    return $global_vars[$type][$key]['value'];
  }
  elseif (!empty($global_vars[$type])) {
    return $global_vars[$type];
  }
  else {
    return FALSE;
  }
}

/**
 * Set act preset, for preset globals var.
 */
function style_manager_set_active_category($cat, $preset) {
  $category = & drupal_static('style_manager_active_category');
  $category = array($cat, $preset);
}

/**
 * Get act preset, for preset globals var.
 */
function style_manager_get_active_preset() {
  $category = & drupal_static('style_manager_active_category');
  if (!empty($_POST['cat']) && empty($category)) {
    $category = explode('--', $_POST['cat']);
  }
  if (!empty($category[1])) {
    $preset = style_manager_preset_get($category[0], $category[1], TRUE);
    return $preset;
  }
  return FALSE;
}

/**
 * Formatting css code.
 */
function style_manager_css_code_formating($css, $add_important = FALSE, $comment = FALSE, $property_filter = FALSE) {
  $settings = array(
    'comment' => $comment,
    'add_important' => $add_important,
    'property_filter' => $property_filter,
  );
  _style_manager_css_selector_formating_preg_callback(NULL, $settings);
  $css = preg_replace_callback('/([^}{]*){/isU', '_style_manager_css_selector_formating_preg_callback', $css);
  _style_manager_css_property_formating_preg_callback(NULL, $settings);
  $css = preg_replace_callback('/{([^}{]*)}/isU', '_style_manager_css_property_formating_preg_callback', $css);
  $css = preg_replace('/([^}{]*{\s*})/isU', '', $css);
  $css = str_replace(array('}', '{'), array("}\r\n\r\n", "{\r\n"), $css);

  if ($comment && trim($css)) {
    $line = '---------------------------------------------------------';
    $css = "\r\n/* {$line}\r\n   {$comment}\r\n   {$line} */\r\n\r\n" . $css;
  }
  return $css;
}

/**
 * Formatting selectors.
 * @see: style_manager_css_code_formating
 */
function _style_manager_css_selector_formating_preg_callback($matches, $settings = NULL) {
  $_settings = & drupal_static(__FUNCTION__);
  if (isset($settings)) {
    $_settings = $settings;
    return FALSE;
  }
  if (empty($matches[1]) || !trim($matches[1])) {
    return '';
  }
  $selectors_str = preg_replace('/\s+/', ' ', $matches[1]);
  if (!empty($_settings['css_prefix']) && trim($_settings['css_prefix'])) {
    $selectors_arr = style_manager_get_css_selectors($_settings['css_prefix'], $selectors_str);
  }
  else {
    $selectors_arr = explode(',', $selectors_str);
  }
  asort($selectors_arr);
  $out = '';
  $selectors_count = count($selectors_arr);
  foreach ($selectors_arr as $selector_data) {
    $selector_data = trim($selector_data);
    if (!$selector_data) {
      continue;
    }
    if ($out) {
      $out .= ',';
      if ($selectors_count < 10) {
        $out .= "\r\n";
      }
    }
    $out .= $selector_data;
  }
  $out .= "{";

  return $out;
}

/**
 * Formatting property.
 * @see: style_manager_css_code_formating
 */
function _style_manager_css_property_formating_preg_callback($matches, $settings = NULL) {
  $_settings = & drupal_static(__FUNCTION__);
  if (isset($settings)) {
    $_settings = $settings;
    return FALSE;
  }
  if (empty($matches[1]) || !trim($matches[1])) {
    return " {}";
  }
  $properties = explode(';', trim($matches[1]));
  $out = '';
  $out .= " {";
  foreach ($properties as $property_data) {
    $property_data = trim($property_data);
    if (!$property_data || style_manager_property_skip($_settings['property_filter'], $property_data)) {
      continue;
    }
    $out .= '  ' . $property_data;
    if ($_settings['add_important'] && stristr($property_data, 'important') === FALSE) {
      $out .= " !important";
    }
    $out .= ";\r\n";
  }
  $out .= "}";

  return $out;
}


/**
 * Using for filter property.
 * @see style_manager_css_code_formating
 */
function style_manager_property_skip($filter_array, $data) {
  if (!$filter_array) {
    return FALSE;
  }
  $found = FALSE;
  foreach ($filter_array as $filter_key) {
    if (stristr($data, $filter_key) !== FALSE) {
      $found = TRUE;
    }
  }
  return !$found;
}

/**
 * Return selector merged with css prefix.
 */
function style_manager_get_css_selectors($prefix_selectors, $el_selectors) {
  $prefix_selectors_arr = explode(',', $prefix_selectors);
  $el_selectors_arr = explode(',', $el_selectors);
  $out_selectors = array();
  foreach ($el_selectors_arr as $el_selector_item) {
    foreach ($prefix_selectors_arr as $prefix_selector_item) {
      $merged_selector = str_replace('{this}', trim($prefix_selector_item), $el_selector_item);
      if ($merged_selector == $el_selector_item) {
        $merged_selector = trim($prefix_selector_item) . ' ' . trim($el_selector_item);
      }
      $merged_selector = trim($merged_selector);
      if ($merged_selector) {
        $out_selectors[$merged_selector] = $merged_selector;
      }
    }
  }
  return $out_selectors;
}

/**
 * Clean css cache.
 */
function style_manager_clean_css_cache() {
  $path = file_build_uri('style_manager_cache/css/');
  $files = file_scan_directory($path, '/.*\.css/');
  if (!empty($files)) {
    foreach ($files as $file_data) {
      unlink($file_data->uri);
    }
  }
}

/**
 * Check path visibility, for allow add editor or css files.
 */
function style_manager_path_visibility_check($variable) {
  $path = drupal_get_path_alias($_GET['q']);
  $page_match = drupal_match_path($path, variable_get($variable, ''));
  if ($path != $_GET['q']) {
    $page_match = $page_match || drupal_match_path($_GET['q'], variable_get($variable, ''));
  }
  return !$page_match;
}

/**
 * Callback for preg_replace_callback.
 * @see style_manager_get_styles_form_json()
 */
function style_manager_replace_callback($matches) {
  $data = preg_replace('/\s+/', ' ', stripslashes($matches[2]));
  return ' ' . $data . ' ';
}
