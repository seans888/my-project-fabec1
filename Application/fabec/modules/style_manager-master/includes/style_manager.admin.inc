<?php

/**
 * @file
 * Serves administration pages of Style manager.
 */

/**
 * Settings form.
 */
function style_manager_settings($form, $form_state) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.helper');
  $form = array();

  $form['clear_cache'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear cache'),
  );

  $form['clear_cache']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear file cache'),
    '#submit' => array('style_manager_delete_cache_files'),
  );

  $form['style_manager_exclude_editor_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude Style Manager editor on paths'),
    '#default_value' => variable_get('style_manager_exclude_editor_paths', "admin*\nimce*"),
    '#description' => t("Exclude the Style Manager editor on some pages, usually administration pages. Enter one page per line as Drupal paths. The '*' character is a wildcard. &lt;front&gt; is the front page."),
    '#wysiwyg' => FALSE,
  );
  $form['style_manager_exclude_css_paths'] = array(
    '#type' => 'textarea',
    '#title' => t("Don't add Style Manager css on paths"),
    '#default_value' => variable_get('style_manager_exclude_css_paths', ''),
    '#description' => t("Do not add styles created by Style Manager on pages. Enter one page per line as Drupal paths. The '*' character is a wildcard. &lt;front&gt; is the front page."),
    '#wysiwyg' => FALSE,
  );

  $form = system_settings_form($form);
  return $form;
}

/**
 * Delete all cache files.
 */
function style_manager_delete_cache_files() {
  $path = file_build_uri('style_manager_cache');
  return file_unmanaged_delete_recursive($path);
}
