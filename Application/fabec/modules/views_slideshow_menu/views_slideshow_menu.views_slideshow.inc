<?php

/**
 *  @file
 *  The default options available with Views Slideshow: Menu.
 */

function views_slideshow_menu_views_slideshow_modes() {
  $options = array(
    'views_slideshow_menu' => t('Menu'),
  );
  return $options;
}

function views_slideshow_menu_views_slideshow_option_definition() {
  $options['views_slideshow_menu'] =array(
    'contains' => array(
      'menu_selector' => '',
      'start_slide_path' => array('default' => 1),
      'wrap_slide_link' => array('default' => 0),
      'timeout' => array('default' => 5000),
      'delay' => array('default' => 0),
      'speed' => array('default' => 700), //normal
      'random' => array('default' => 0),
      'pause' => array('default' => 1),
      'effect' => array('default' => 'fade'),
      'sync' => array('default' => 1),
      'ie' => array(
        'contains' => array(
          'cleartype' => array('default' => TRUE),
          'cleartypenobg' => array('default' => FALSE),
        ),
      ),
      'advanced' => array('default' => ''),
    ),
  );
  return $options;
}

function views_slideshow_menu_views_slideshow_options_form(&$form, &$form_state, &$view) {
  $form['views_slideshow_menu']['js'] = array(
    '#value' => "<script type='text/javascript'>
                  viewsSlideshowMenuSelectorToggle();
                  $('#edit-style-options-menu-menus').change(function() {
                    viewsSlideshowMenuSelectorToggle();
                  });
                  
                  function viewsSlideshowMenuSelectorToggle() {
                    if ($('#edit-style-options-menu-menus').val() == 0) {
                      $('#edit-style-options-menu-menu-selector-wrapper').show('normal');
                    }
                    else {
                      $('#edit-style-options-menu-menu-selector-wrapper').hide('normal');
                    }
                  }
                </script>",
  );

  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title");
  $options[] = 'Use Selector';
  while($menu = db_fetch_array($result)) {
    $options[$menu['menu_name']] = $menu['title'];
  }
  $form['views_slideshow_menu']['menus'] = array(
    '#type' => 'select',
    '#title' => t('Menu Blocks'),
    '#options' => $options,
    '#default_value' => $view->options['views_slideshow_menu']['menus'],
    '#description' => t('Choose the menu you would like the slideshow to be associated with. This menu will have to be added to the page as a block.  Choose "Use Selector" to specify a css path to a menu.'),
  );
  $form['views_slideshow_menu']['menu_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Selector'),
    '#default_value' => $view->options['views_slideshow_menu']['menu_selector'],
    '#description' => t('Add a css selector to choose a menu that is not in a block. Ex. .primary-links'),
  );
  $form['views_slideshow_menu']['start_slide_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start Slideshow On Matching Menu Path'),
    '#default_value' => $view->options['views_slideshow_menu']['start_slide_path'],
    '#description' => t('When the user clicks a menu item start the slideshow menu on the menu item clicked.'),
  );
  $form['views_slideshow_menu']['wrap_slide_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link the slide to the menu URL'),
    '#default_value' => $view->options['views_slideshow_menu']['wrap_slide_link'],
    '#description' => t('Make the slide link to the menu item.'),
  );
  $form['views_slideshow_menu']['timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timer delay'),
    '#default_value' => $view->options['views_slideshow_menu']['timeout'],
    '#description' => t('Amount of time in milliseconds between transitions.')
  );
  $form['views_slideshow_menu']['delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial slide delay offset'),
    '#default_value' => $view->options['views_slideshow_menu']['delay'],
    '#description' => t('Amount of time in milliseconds for the first slide to transition. This number will be added to Timer delay to create the initial delay.  For example if Timer delay is 4000 and Initial delay is 2000 then the first slide will change at 6000ms (6 seconds).  If Initial delay is -2000 then the first slide will change at 2000ms (2 seconds).')
  );
  $form['views_slideshow_menu']['speed'] = array(
    '#type' => 'textfield',
    '#title' => t('Speed'),
    '#default_value' => $view->options['views_slideshow_menu']['speed'],
    '#description' => t('Time in milliseconds that each transition lasts. Numeric only!'),
  );
  $form['views_slideshow_menu']['random'] = array(
    '#type' => 'radios',
    '#title' => t('Random'),
    '#description' => t('This option controls the order items are displayed. The default setting, False, uses the views ordering. True will cause the images to display in a random order.'),
    '#options' => array(0 => t('False'), 1 => t('True')),
    '#default_value' => $view->options['views_slideshow_menu']['random'],
  );
  $form['views_slideshow_menu']['pause'] = array(
    '#type' => 'radios',
    '#title' => t('Pause'),
    '#options' => array(1 => t('Yes'),2 => t('No')),
    '#default_value' => $view->options['views_slideshow_menu']['pause'],
    '#description' => t('Pause when hovering on the slideshow image.'),
  );
  $options = array(
    'none' => 'none',
    'blindX' => 'blindX',
    'blindY' => 'blindY',
    'blindZ' => 'blindZ',
    'cover' => 'cover',
    'curtainX' => 'curtainX',
    'curtainY' => 'curtainY',
    'fade' => 'fade',
    'fadeZoom' => 'fadeZoom',
    'growX' => 'growX',
    'growY' => 'growY',
    'scrollUp' => 'scrollUp',
    'scrollDown' => 'scrollDown',
    'scrollLeft' => 'scrollLeft',
    'scrollRight' => 'scrollRight',
    'scrollHorz' => 'scrollHorz',
    'scrollVert' => 'scrollVert',
    'shuffle' => 'shuffle',
    'slideX' => 'slideX',
    'slideY' => 'slideY',
    'toss' => 'toss',
    'turnUp' => 'turnUp',
    'turnDown' => 'turnDown',
    'turnLeft' => 'turnLeft',
    'turnRight' => 'turnRight',
    'uncover' => 'uncover',
    'wipe' => 'wipe',
    'zoom' => 'zoom'
  );
  $form['views_slideshow_menu']['effect'] = array(
    '#type' => 'select',
    '#title' => t('Effect'),
    '#options' => $options,
    '#default_value' => $view->options['views_slideshow_menu']['effect'],
    '#description' => t('The transition effect that will be used to change between images. Not all options below may be relevant depending on the effect.'),
  );
  $form['views_slideshow_menu']['sync'] = array(
    '#type' => 'radios',
    '#title' => t('Sync'),
    '#options' => array(1 => t('Yes'), 2 => t('No')),
    '#default_value' => $view->options['views_slideshow_menu']['sync'],
    '#description' => t('The sync option controls whether the slide transitions occur simultaneously. The default is yes which means that the current slide transitions out as the next slide transitions in. By setting the sync option to no you can get some interesting twists on your transitions.'),
  );
  $form['views_slideshow_menu']['advanced'] = array(
    '#type' => 'textarea',
    '#title' => t('Advanced Options'),
    '#default_value' => $view->options['views_slideshow_menu']['advanced'],
    '#description' => t('Add other jQuery cycle options one per line.  Ex. height: 350  !url', array('!url' => l('Click here for the additional options to add.', 'http://malsup.com/jquery/cycle/options.html'))),
  );
  $form['views_slideshow_menu']['ie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Internet Explorer Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['views_slideshow_menu']['ie']['cleartype'] = array(
    '#type' => 'radios',
    '#title' => t('ClearType'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
    '#default_value' => $view->options['views_slideshow_menu']['ie']['cleartype'],
    '#description' => t('True if clearType corrections should be applied (for IE).  Some background issues could be fixed by setting this to false.'),
  );
  $form['views_slideshow_menu']['ie']['cleartypenobg'] = array(
    '#type' => 'radios',
    '#title' => t('ClearType Background'),
    '#options' => array('true' => t('True'), 'false' => t('False')),
    '#default_value' => $view->options['views_slideshow_menu']['ie']['cleartypenobg'],
    '#description' => t('Set to true to disable extra cleartype fixing (leave false to force background color setting on slides)'),
  );
}

function views_slideshow_menu_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_menu']['speed'])) {
    form_error($form['views_slideshow_menu']['speed'], t('!setting must be numeric!',array('Speed')));
  }
  if (!is_numeric($form_state['values']['style_options']['views_slideshow_menu']['timeout'])) {
    form_error($form['views_slideshow_menu']['speed'], t('!setting must be numeric!',array('timeout')));
  }
}
