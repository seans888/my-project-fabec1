<?php

/**
 *  @file
 *  Theme & preprocess functions for the Views Slideshow: Menu module.
 */

/**
 *  Implement template_preprocess_hook_THEMENAME().
 */
function template_preprocess_views_slideshow_menu(&$vars) {
  $options = $vars['options'];
  $base = drupal_get_path('module', 'views_slideshow_menu');
  drupal_add_js($base . '/views_slideshow.js', 'module');
  drupal_add_css($base . '/views_slideshow.css', 'module');

  $num_divs = sizeof($vars['rows']);

  $options['views_slideshow_menu']['menu_selector'] = trim($options['views_slideshow_menu']['menu_selector'], ',');

  if (empty($options['views_slideshow_menu']['menu_selector'])) {
    $options['views_slideshow_menu']['menu_selector'] = '#block-menu-' . $options['views_slideshow_menu']['menus'];
  }

  $settings = array_merge(
    array(
      'num_divs' => $num_divs,
      'id_prefix' => '#views_slideshow_menu_main_',
      'div_prefix' => '#views_slideshow_menu_div_',
      'id' => $vars['id'],
    ),
    $options['views_slideshow_menu']
  );

  drupal_add_js(array('viewsSlideshowMenu' => array('#views_slideshow_menu_main_'. $vars['id'] => $settings)), 'setting');

}


/**
 *  These are the slideshow elements themselves; not actually displayed, but used to give the html to the main element.
 */
function theme_views_slideshow_menu_no_display_section($view, $rows, $id, $mode, $teaser = TRUE) {
  // Add support for the jQuery Cycle plugin.
  // If we have the jQ module installed, use that to add the Cycle plugin if possible.
  // That allows for version control.
  if (module_exists('jq')) {
    $loaded_plugins = jq_plugins();
    if (!empty($loaded_plugins['cycle'])) {
      $js = jq_add('cycle');
    }
  }

  // Otherwise, we'll add the version included with this module.
  if (!$js) {
    drupal_add_js(drupal_get_path('module', 'views_slideshow') .'/js/jquery.cycle.all.min.js', 'module');
  }

  $output = '<div id="views_slideshow_menu_teaser_section_' . $id . '" class="views_slideshow_menu_teaser_section">' . "\n";

  foreach ($rows as $count => $item) {
    $output .= theme('views_slideshow_menu_no_display_teaser', $item, $id, $count);
  }
  $output .= "</div><!--close views_slideshow_menu_no_display-->\n\n";
  return $output;
}

/**
 *  The html that will be placed into the element in its turn during its frame.
 */
function theme_views_slideshow_menu_no_display_teaser($item, $id, $count) {
  $class = 'views_slideshow_menu_slide views_slideshow_slide';
  if ($count) {
    $class .= ' views_slideshow_menu_hidden';
  }

  if ($count % 2) {
    $class .= ' views-row-even';
  }
  else {
    $class .= ' views-row-odd';
  }
  $output = '  <div id="views_slideshow_menu_div_' . $id . '_' . $count . '" class="' . $class .'">' . "\n  ";
  $output .= $item . "\n";
  $output .= '  </div><!--close views_slideshow_menu_div_' . $id . '_' . $count . '-->' . "\n\n";
  return $output;
}
