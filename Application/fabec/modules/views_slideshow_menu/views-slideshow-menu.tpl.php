<?php

/**
 *  @file
 *  Views Slideshow: Menu
 */

  // these are hidden elements, used to cycle through the main div
  $hidden_elements = theme('views_slideshow_menu_no_display_section', $view, $rows, $id, $options['mode'], $teaser);
  print theme('views_slideshow_main_section', $id, $hidden_elements, 'views_slideshow_menu');
