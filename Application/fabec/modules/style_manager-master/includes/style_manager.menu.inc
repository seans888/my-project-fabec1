<?php

/**
 * @file
 * Menu callbacks.
 */

/**
 * Set preset as default.
 */
function style_manager_set_default_preset_json() {
  $cat_arr = explode('--', $_POST['cat']);
  style_manager_set_default_preset($cat_arr[0], $cat_arr[1]);
}

/**
 * Get css code for live preview.
 */
function style_manager_get_css_json() {
  module_load_include('inc', 'style_manager', 'includes/style_manager.render');
  $cat_arr = explode('--', $_POST['cat']);
  $out = array();

  if (!empty($cat_arr[0]) && empty($cat_arr[1])) {
    $cat_arr[1] = style_manager_get_default_preset($cat_arr[0]);
  }
  if (empty($cat_arr[0]) || $cat_arr[0] == 'global_config') {
    $cat_tree = _style_manager_get_cat_tree();
    if (!empty($cat_tree)) {
      $or_preset = style_manager_get_default_preset('global_config');
      if (!empty($cat_arr[1])) {
        style_manager_set_default_preset('global_config', $cat_arr[1]);
      }
      foreach ($cat_tree as $id => $title) {
        if ($id == 'global_config') {
          continue;
        }
        $preset = style_manager_get_default_preset($id);
        $out[] = array(
          'cat' => $id,
          'css' => style_manager_preset_to_css($id, $preset),
        );
      }
      style_manager_set_default_preset('global_config', $or_preset);
    }
  }
  else {
    $out[] = array(
      'cat' => $cat_arr[0],
      'css' => style_manager_preset_to_css($cat_arr[0], $cat_arr[1]),
    );
  }
  $reset_all = ($cat_arr[0] == 'global_config') ? TRUE : FALSE;
  style_manager_print_json(array('data' => $out, 'reset_all' => $reset_all));
}

/**
 * Rename preset.
 */
function style_manager_preset_save_as_json() {
  $cat = $_POST['cat'];
  $cat_arr = explode('--', $cat);
  $title = $_POST['title'];
  $id = md5($_POST['title']);

  $preset = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  $preset['info']['preset_title'] = $title;
  style_manager_preset_set($cat_arr[0], $id, $preset);
  style_manager_print_json(array(
    'message' => '',
    'new_id' => $cat_arr[0] . '--' . $id,
    'old_id' => $cat,
  ));
}

/**
 * Save visibility settings for element in selected preset and group.
 */
function style_manager_preset_item_visibility_settings_save_json() {
  $cat = $_POST['cat'];
  $key = $_POST['key'];
  $group = $_POST['group'];
  $value = $_POST['value'];
  if ($value == 'false') {
    $value = FALSE;
  }
  if ($value == 'true') {
    $value = TRUE;
  }
  $cat_arr = explode('--', $cat);
  $preset = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  $preset['info']['visibility_settings'][$group][$key] = $value;
  style_manager_preset_set($cat_arr[0], $cat_arr[1], $preset);
  style_manager_print_json(array(
    'message' => '',
    'cat_id' => $cat,
    'key' => $key,
  ));
}

/**
 * Save visibility settings for all elements in selected preset and group.
 */
function style_manager_preset_item_visibility_settings_save_all_json() {
  $cat = $_POST['cat'];
  $key = $_POST['key'];
  $group = $_POST['group'];
  $visibility_settings = json_decode($_POST['visibility_settings'], TRUE);
  $cat_arr = explode('--', $cat);
  $preset = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  $preset['info']['visibility_settings'][$group] = $visibility_settings;
  style_manager_preset_set($cat_arr[0], $cat_arr[1], $preset);
  style_manager_print_json(array(
    'message' => '',
    'cat_id' => $cat,
    'key' => $key,
  ));
}

/**
 * Rename preset.
 */
function style_manager_preset_rename_json() {
  $cat_arr = explode('--', $_POST['cat']);
  $title = $_POST['title'];
  $id = md5($_POST['title']);
  $default_preset = style_manager_get_default_preset($cat_arr[0]);

  if ($default_preset == $cat_arr[1]) {
    style_manager_set_default_preset($cat_arr[0], $id);
  }
  if ($id == $cat_arr[1]) {
    style_manager_print_json(array('message' => 'Error'));
  }

  $preset_user = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  $preset = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  style_manager_preset_delete($cat_arr[0], $cat_arr[1]);
  style_manager_preset_delete($cat_arr[0], $cat_arr[1]);
  $preset_user['info']['preset_title'] = $title;
  $preset['info']['preset_title'] = $title;
  style_manager_preset_set($cat_arr[0], $id, $preset_user);
  style_manager_preset_set($cat_arr[0], $id, $preset);

  style_manager_print_json(array(
    'message' => '',
    'new_id' => $cat_arr[0] . '--' . $id,
  ));
}

/**
 * Delete preset.
 */
function style_manager_preset_delete_json() {
  $cat_arr = explode('--', $_POST['cat']);
  $default_preset = style_manager_get_default_preset($cat_arr[0]);
  if ($default_preset == $cat_arr[1]) {
    style_manager_set_default_preset($cat_arr[0], 'default');
  }
  style_manager_preset_delete($cat_arr[0], $cat_arr[1]);
  style_manager_print_json(array('message' => '', 'cat_id' => $cat_arr[0]));
}


/**
 * Get main form with css settings for selected preset and group.
 */
function style_manager_get_styles_form_json() {
  module_load_include('inc', 'style_manager', 'includes/style_manager.js-form');
  $all_elements = array();
  $group = $_POST['group'];
  $cat = $_POST['cat'];
  $cat_arr = explode('--', $_POST['cat']);
  $form = array();
  $cat_info = style_manager_get_cat_info($cat_arr[0], FALSE, 'info');
  $cat_data = style_manager_get_cat_info($cat_arr[0], FALSE, 'data');
  $preset_data = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  foreach ($cat_data as $key => $data) {
    $selector[$key] = $data['selector'];
    $type[$key] = $data['type'];
    $title[$key] = $data['title'];
    $weight[$key] = empty($data['weight']) ? 0 : $data['weight'];
  }
  array_multisort($weight, SORT_ASC, $title, SORT_ASC, $type, SORT_ASC, $selector, SORT_ASC, $cat_data);
  foreach ($cat_data as $key => $data) {
    if ($data['group'] == $group) {
      $fn = style_manager_get_widget_info($data['type'], 'form');
      if (function_exists($fn)) {
        $data['title'] = $data['title'] ? $data['title'] . ' - ' : '';
        $data['title'] .= style_manager_get_widget_info($data['type'], 'title');
        if ($data['selector']) {
          if (strlen($data['selector']) > 35) {
            $selectors_title = '<span title="' . $data['selector'] . '">' . substr($data['selector'], 0, 32) . '...<span>';
          }
          else {
            $selectors_title = $data['selector'];
          }
          $data['title'] .= ' (' . $selectors_title . ')';
        }
        $data['id'] = $key;
        if (!empty($preset_data['data'][$key])) {
          $data += $preset_data['data'][$key];
        }
        else {
          $data += array('settings' => style_manager_get_widget_info($data['type'], 'default'));
        }
        $all_elements[] = array(
          'id' => $data['id'],
          'text' => $data['title'],
          'checked' => !empty($preset_data['info']['visibility_settings'][$group][$data['id']]),
        );
        $data['output_selectors'] = style_manager_get_css_selectors($cat_info['css_prefix'], $data['selector']);
        $cat_arr = explode('--', $_POST['cat']);
        if (style_manager_get_item_visibility_status($cat_arr[0], $cat_arr[1], $group, $data['id'])) {
          $str_data = serialize($data);
          $replace_arr = array('<js_start>', '<js_end>');
          $str_data = str_replace($replace_arr, '', $str_data);
          $form[] = call_user_func_array($fn, array(array('data' => unserialize($str_data))));
        }
      }
    }
  }
  $presets = style_manager_get_presets($cat_arr[0], FALSE);
  $visibility_type = !empty($preset_data['info']['visibility_settings'][$group]['visibility_type']) ? $preset_data['info']['visibility_settings'][$group]['visibility_type'] : 'show_all';
  $json = json_encode(array(
    'form' => $form,
    'presets' => $presets,
    'cat_id' => $cat,
    'visibility_settings' => $all_elements,
    'visibility_type' => $visibility_type,
  ));

  $json = preg_replace_callback('/\"(\s*<js_start>)(.*)(<js_end>\s*)\"/isU', 'style_manager_replace_callback', $json);
  print $json;
}


/**
 * Save style settings.
 */
function style_manager_save_styles_json() {
  $cat_arr = explode('--', $_POST['cat']);
  $field_values = (array) json_decode($_POST['field_values']);
  $user_preset = style_manager_preset_get($cat_arr[0], $cat_arr[1]);
  if ($field_values && $user_preset) {
    foreach ($field_values as $key => $val) {
      $key_arr = explode('--', $key);
      $store_el = & $user_preset['data'];
      $i = count($key_arr);
      if ($key_arr[1] != 'settings') {
        continue;
      }
      foreach ($key_arr as $key_val) {
        $i--;
        if ($i > 0) {
          if (empty($store_el[$key_val]) || is_string($store_el[$key_val])) {
            $store_el[$key_val] = array();
          }
          $store_el = & $store_el[$key_val];
        }
        else {
          $store_el[$key_val] = $val;
        }
      }
    }
  }
  else {
    style_manager_print_json(array('message' => t("Error. Can't save styles.")));
    return;
  }
  style_manager_preset_set($cat_arr[0], $cat_arr[1], $user_preset);
  style_manager_print_json(array('message' => ''));
}


/**
 * Get category tree.
 */
function style_manager_get_cat_list_json() {
  $selected_cat = isset($_POST['selected_cat']) ? $_POST['selected_cat'] : FALSE;
  $out_data = array();
  $cat_tree = _style_manager_get_cat_tree();
  asort($cat_tree);

  // global_config is first.
  $global_config = $cat_tree['global_config'];
  unset($cat_tree['global_config']);
  $cat_tree = array('global_config' => $global_config) + $cat_tree;
  if (!empty($cat_tree)) {
    foreach ($cat_tree as $id => $title) {
      $item = array();
      $item['text'] = $title . '<a href="#" class="category_menu" data-id="' . $id . '"></a>';
      $item['id'] = $id;
      $children = style_manager_get_presets($id);
      if ($children) {
        if ($selected_cat == $id) {
          $item['expanded'] = TRUE;
        }
        else {
          $item['expanded'] = FALSE;
        }
        $item['children'] = $children;
      }
      else {
        $item['leaf'] = TRUE;
      }
      $out_data[] = $item;
    }
  }
  style_manager_print_json($out_data);
}


/**
 * Get presets list.
 */
function style_manager_get_presets($cat_id, $mark_default = TRUE) {
  $out_data = array();
  $default_preset = style_manager_get_default_preset($cat_id);
  $presets = style_manager_get_all_presets($cat_id);
  $title = array();
  $default_first = array();
  foreach ($presets as $key => $data) {
    $title[$key] = $data['info']['preset_title'];
    $default_first[$key] = 1;
  }
  $default_first['default'] = '0';
  array_multisort($default_first, SORT_ASC, $title, SORT_ASC, $presets);
  if ($cat_id != 'global_config') {
    $presets['disable'] = array('info' => array('preset_title' => t('Disabled')));
  }
  if ($presets) {
    foreach ($presets as $key => $data) {
      $tree_item = array(
        'text' => $data['info']['preset_title'],
        'id' => $cat_id . '--' . $key,
        'leaf' => TRUE,
        'iconCls' => 'preset_icon',
      );
      if ($default_preset == $key && $mark_default) {
        $tree_item['text'] = '<span  data-qtip="' . t('Default preset.') . '">' . $tree_item['text'] . '</span>';
        $tree_item['iconCls'] = 'act_preset_icon';
        $tree_item['default'] = TRUE;
      }
      elseif (style_manager_preset_is_override($cat_id, $key)) {
        $tree_item['text'] = '<span data-qtip="' . style_manager_preset_is_override($cat_id, $key) . '">* ' . $tree_item['text'] . '</span>';
      }
      $tree_item['text'] .= ' <a href="#" class="preset_menu" data-id="' . $tree_item['id'] . '"></a>';
      $out_data[] = $tree_item;
    }
  }
  return $out_data;
}

/**
 * Get groups for category.
 */
function style_manager_get_category_groups_json() {
  $cat_arr = explode('--', $_GET['cat']);
  $cat_el = style_manager_get_cat_info($cat_arr[0], FALSE, 'data');
  if (!$cat_el) {
    style_manager_print_json(array());
    exit();
  }
  $sort_title = array();
  $sort_locked = array();
  $out_data = array();
  if ($cat_arr[1] != 'disable') {
    foreach ($cat_el as $data) {
      $sort[$data['group']] = 0;
      if (!empty($data['locked'])) {
        $sort_locked[$data['group']] = 1;
      }
      elseif (empty($sort[$data['group']])) {
        $sort_locked[$data['group']] = 2;
      }
      $sort_title[$data['group']] = $data['group'];
      $out_data[$data['group']] = array(
        'title' => $data['group'],
        'status' => TRUE,
      );
    }
  }
  array_multisort($sort_locked, SORT_ASC, $sort_title, SORT_ASC, $out_data);
  style_manager_print_json(array_values($out_data));
}

/**
 * Get list of all elements for manage it, in category settings.
 */
function style_manager_category_form_item_load() {
  $cat_id = arg(3);
  $group_id = arg(4);
  $preset = style_manager_get_cat_info($cat_id, FALSE, 'data');
  $out = array();
  if (isset($preset)) {
    foreach ($preset as $key => $data) {
      if (empty($group_id) || ($group_id == $data['group'])) {
        $out[] = array(
          'id' => $key,
          'title' => $data['title'],
          'type' => $data['type'],
          'group' => $data['group'],
          'locked' => !empty($data['locked']),
          'selector' => isset($data['selector']) ? $data['selector'] : '',
          'weight' => isset($data['weight']) ? $data['weight'] : 0,
        );
      }
    }
  }
  style_manager_print_json($out);
}

/**
 * Save or update element settings.
 */
function style_manager_category_form_item_save($cat_id = FALSE, $selected_cat = FALSE, $fields = FALSE) {
  if (!$fields) {
    $cat_id = arg(3);
    $selected_cat = arg(4);
    $post_data = json_decode($GLOBALS['HTTP_RAW_POST_DATA']);
    $fields = (array) $post_data->data;
  }

  if (isset($fields[0]->type)) {
    foreach ($fields as $data) {
      $data = (array) $data;
      style_manager_category_form_item_save($cat_id, $selected_cat, $data);
      return;
    }
  }
  $preset_id = FALSE;
  if (!empty($selected_cat)) {
    $selected_cat_arr = explode('--', $selected_cat);
    if (!empty($selected_cat_arr)) {
      $preset_id = $selected_cat_arr[1];
    }
  }
  if (empty($fields['type']) || empty($fields['group'])) {
    print '{"success":false, "message":"Error."}';
    return;
  }
  if ($fields['title'] == '' && in_array($fields['type'], array(
    'style_manager_global_color',
    'style_manager_global_font',
    'style_manager_global_numeric',
  ))
  ) {
    $fields['title'] = t('Untitled');
  }
  if (in_array($fields['type'], array(
    'css_code',
    'style_manager_global_color',
    'style_manager_global_font',
    'style_manager_global_numeric',
  ))
  ) {
    $fields['selector'] = '';
  }
  elseif ($fields['selector'] == '') {
    $fields['selector'] = '{this}';
  }
  if ($fields['type'] == 'reset' && $fields['weight'] == '') {
    $fields['weight'] = -1000;
  }
  elseif ($fields['weight'] == '') {
    $fields['weight'] = 0;
  }

  $el_id = !(empty($fields['id'])) ? $fields['id'] : md5(time() + serialize($fields));
  unset($fields['id']);
  $info = style_manager_get_cat_info($cat_id, FALSE, 'data');
  $is_new = (!isset($info[$el_id]) || $info[$el_id]['type'] != $fields['type']);
  $info[$el_id] = $fields;
  style_manager_set_cat_info($cat_id, $info, 'data');
  if ($is_new) {
    $all_presets = style_manager_get_all_presets($cat_id);
    foreach ($all_presets as $key => $preset) {
      if ($key == $preset_id && !empty($preset['info']['visibility_settings'][$fields['group']]['visibility_type']) && $preset['info']['visibility_settings'][$fields['group']]['visibility_type'] == 'show_selected'
      ) {
        $preset['info']['visibility_settings'][$fields['group']][$el_id] = TRUE;
      }
      $preset['data'][$el_id]['settings'] = style_manager_get_widget_info($fields['type'], 'default');
      style_manager_preset_set($cat_id, $key, $preset);
    }
  }
  $fields['id'] = $el_id;
  $response = array(
    'success' => TRUE,
    'data' => $fields,
  );
  style_manager_print_json($response);
}

/**
 * Delete element.
 */
function style_manager_category_form_item_destroy() {
  $cat_id = arg(3);
  $post_data = json_decode($GLOBALS['HTTP_RAW_POST_DATA']);
  if (empty($post_data->data->id)) {
    print '{"success":false}';
    return FALSE;
  }
  $el_id = $post_data->data->id;
  $info = style_manager_get_cat_info($cat_id, FALSE, 'data');
  unset($info[$el_id]);
  style_manager_set_cat_info($cat_id, $info, 'data');
  $all_presets = style_manager_get_all_presets($cat_id);
  foreach ($all_presets as $key => $preset) {
    unset($preset['data'][$el_id]);
    style_manager_preset_set($cat_id, $key, $preset);
  }
  print '{"success":true}';
}

/**
 * Get category info.
 */
function style_manager_category_load() {
  $cat_id = $_POST['cat'];
  $info = style_manager_get_cat_info($cat_id);
  style_manager_print_json($info);
}

/**
 * Save category info.
 */
function style_manager_category_save() {
  $cat_id = $_POST['cat_id'];
  $cat_title = $_POST['cat_title'];
  $css_prefix = $_POST['css_prefix'];
  $add_important = $_POST['add_important'];
  $old_cat_id = $_POST['old_cat_id'];
  $out = array();
  if (empty($add_important) || $add_important == 'false') {
    $add_important = FALSE;
  }
  else {
    $add_important = TRUE;
  }
  if (empty($_POST['cat_id']) || empty($_POST['cat_title'])) {
    style_manager_print_json(array('error' => t('Error.')));
    return;
  }
  if (empty($old_cat_id)) {
    $i = 1;
    $orig_cat_id = $cat_id;
    while (style_manager_get_cat_info($cat_id)) {
      $cat_id = $orig_cat_id . '_' . $i++;
    }
    style_manager_set_cat_info($cat_id, array(
      'id' => $cat_id,
      'title' => $cat_title,
      'css_prefix' => $css_prefix,
      'add_important' => $add_important,
    ), 'info');
    style_manager_set_cat_info($cat_id, array(), 'data');
    style_manager_preset_set($cat_id, 'default', array(
      'info' => array('preset_title' => 'Default'),
      'data' => array(),
    ));
    $out['id'] = $cat_id;
    cache_set('style_manager_cat_list', FALSE);
  }
  else {
    $info = style_manager_get_cat_info($cat_id, FALSE);
    $info['title'] = $cat_title;
    $info['css_prefix'] = $css_prefix;
    $info['add_important'] = $add_important;
    style_manager_set_cat_info($cat_id, $info);
    $out['id'] = $cat_id;
  }
  style_manager_print_json($out);
}

/**
 * Reset category settings.
 */
function style_manager_category_reset() {
  $cat_id = $_POST['cat_id'];
  style_manager_cat_delete($cat_id);
  cache_set('style_manager_presets_folder_key', '');
  style_manager_categoryes_init();
  style_manager_print_json(array('id' => $cat_id));
}

/**
 * Destroy category.
 */
function style_manager_category_destroy() {
  $cat_id = $_POST['cat_id'];
  style_manager_cat_delete($cat_id);
  cache_set('style_manager_cat_list', FALSE);
  style_manager_print_json(array('id' => $cat_id));
}

/**
 * Export category to file with json code.
 */
function style_manager_category_export() {
  module_load_include('inc', 'style_manager', 'includes/style_manager.image');
  $cat_id = arg(2);
  $images = array();
  $out = array();
  if (!$cat_id) {
    print '';
    return;
  }
  $out['module_version'] = style_manager_get_version();
  $out['info'] = style_manager_get_cat_info($cat_id, FALSE, 'info');
  $out['data'] = style_manager_get_cat_info($cat_id, FALSE, 'data');
  $out['preset_data'] = style_manager_get_cat_info($cat_id, FALSE, 'preset_data');
  if ($out['preset_data']) {
    foreach ($out['preset_data'] as &$preset) {
      $images += style_manager_images_export($preset['data']);
    }
  }
  $out['images'] = $images;
  header('Content-Type: application/x-json');
  style_manager_print_json($out);
}

/**
 * Import category from file.
 */
function style_manager_category_import() {
  if (empty($_FILES['settings']['tmp_name'])) {
    style_manager_print_json(array(
      'success' => FALSE,
      'message' => t("Can't find file."),
    ));
    return;
  }
  $settings_str = fread(fopen($_FILES['settings']['tmp_name'], "r"), $_FILES['settings']['size']);
  module_load_include('inc', 'style_manager', 'includes/style_manager.image');
  $cat_id = arg(2);
  $category_arr = json_decode($settings_str, TRUE);
  if (!$category_arr || !isset($category_arr['data']) || empty($category_arr['info']['id']) || empty($cat_id)) {
    style_manager_print_json(array(
      'success' => FALSE,
      'message' => t('Can not import this file.'),
    ));
  }
  else {
    $category_arr['info']['id'] = $cat_id;
    $category_arr['info']['title'] = style_manager_get_cat_info($cat_id, 'title');
    style_manager_set_cat_info($category_arr['info']['id'], $category_arr['info'], 'info');
    style_manager_set_cat_info($category_arr['info']['id'], $category_arr['data'], 'data');
    if (!empty($category_arr['images'])) {
      foreach ($category_arr['preset_data'] as &$preset) {
        $preset['data'] = style_manager_images_import($preset['data'], $category_arr['images']);
      }
    }
    style_manager_set_cat_info($category_arr['info']['id'], $category_arr['preset_data'], 'preset_data');
    style_manager_print_json(array(
      'success' => TRUE,
      'id' => $cat_id,
      'message' => t('Import was successful.'),
    ));
  }
}

/**
 * Import styles in css widget.
 */
function style_manager_css_widget_import_css() {
  if (empty($_POST['styles']) || empty($_POST['type'])) {
    return FALSE;
  }
  $styles = $_POST['styles'];
  $type = $_POST['type'];
  $all_contents = '';
  foreach ($styles as $key => $var) {
    $file_content = drupal_load_stylesheet($key, TRUE);
    $css_base_url = file_create_url($key);
    $css_base_url = substr($css_base_url, 0, strrpos($css_base_url, '/'));
    if (substr($css_base_url, 0, strlen($GLOBALS['base_root'])) == $GLOBALS['base_root']) {
      $css_base_url = substr($css_base_url, strlen($GLOBALS['base_root']));
    }
    _drupal_build_css_path(NULL, $css_base_url . '/');
    $all_contents .= preg_replace_callback('/url\(\s*[\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\s*\)/i', '_drupal_build_css_path', $file_content);
  }
  if ($type == 'image_color') {
    $property_filter = array('url', '#', 'rgb');
    $all_contents = style_manager_css_code_formating($all_contents, FALSE, FALSE, $property_filter);
  }
  else {
    $all_contents = style_manager_css_code_formating($all_contents);
  }
  style_manager_print_json(array('success' => TRUE, 'css' => $all_contents));
}
