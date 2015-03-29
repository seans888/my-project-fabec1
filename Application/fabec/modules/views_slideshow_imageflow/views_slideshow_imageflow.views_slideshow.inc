<?php
// $Id: views_slideshow_imageflow.views_slideshow.inc,v 1.1.2.3 2009/06/19 21:06:46 aaron Exp $

/**
 *  @file
 *  The default options available with Views Slideshow: ImageFlow.
 */

/**
 *  Implements hook_views_slideshow_modes().
 */
function views_slideshow_imageflow_views_slideshow_modes() {
  // Expose the mode for ImageFlow slide shows.
  $options = array(
    'imageflow' => t('ImageFlow'),
  );
  return $options;
}

/**
 *  Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_imageflow_views_slideshow_option_definition() {
  // Set our default options.
  $options = array(
    'imageflow' => array(
      'default' => array(
        'aspect_ratio' => '1.964',
        'images_height' => '0.67',
        'image_cursor' => 'pointer',
        'slider_cursor' => 'pointer',
        'slider' => TRUE,
        'captions' => TRUE,
        'imageFocusM' => '1.0',
        'scrollbarP' => '0.6',
        'imageFocusMax' => '4',
      ),
    ),
  );
  return $options;
}

/**
 *  Implements hook_views_slideshow_options_form().
 */
function views_slideshow_imageflow_views_slideshow_options_form(&$form, &$form_state, &$view) {
  // Create the form elements for our ImageFlow view options.
  $form['imageflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('ImageFlow options'),
    '#collapsible' => TRUE,
    '#collapsed' => !($view->options['mode'] == 'imageflow'),
  );
  $form['imageflow']['aspect_ratio'] = array(
    '#type' => 'textfield',
    '#title' => t('Aspect ratio'),
    '#description' => t('Aspect ratio of the ImageFlow container (width divided by height).'),
    '#default_value' => $view->options['imageflow']['aspect_ratio'],
  );
  $form['imageflow']['images_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Images height'),
    '#description' => t('Height of the images div container to be multiplied.'),
    '#default_value' => $view->options['imageflow']['images_height'],
  );
  $form['imageflow']['image_cursor'] = array(
    '#type' => 'select',
    '#title' => t('Image cursor'),
    '#description' => t('Cursor type for the images.'),
    '#default_value' => $view->options['imageflow']['image_cursor'],
    '#options' => array(
      'pointer' => t('pointer'),
      'default' => t('default'),
      'e-resize' => t('e-resize'),
    ),
  );
  $form['imageflow']['slider'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show slider'),
    '#description' => t('Disables / enables the scrollbar.'),
    '#default_value' => $view->options['imageflow']['slider'],
  );
  $form['imageflow']['slider_cursor'] = array(
    '#type' => 'select',
    '#title' => t('Slider cursor'),
    '#description' => t('Cursor type for the slider.'),
    '#default_value' => $view->options['imageflow']['slider_cursor'],
    '#options' => array(
      'pointer' => t('pointer'),
      'default' => t('default'),
      'e-resize' => t('e-resize'),
    ),
  );
  $form['imageflow']['start'] = array(
    '#type' => 'select',
    '#title' => t('Start position'),
    '#description' => t('The position to begin the ImageFlow.'),
    '#default_value' => $view->options['imageflow']['start'],
    '#options' => array(
      'start' => t('Start'),
      'middle' => t('Middle'),
      'end' => t('End'),
      'random' => t('Random'),
    ),
  );
  $form['imageflow']['captions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display captions'),
    '#description' => t('Disables / enables the display of captions.'),
    '#default_value' => $view->options['imageflow']['captions'],
  );
  $form['imageflow']['imageFocusM'] = array(
    '#type' => 'textfield',
    '#title' => t('Image focus multiplier'),
    '#description' => t('Multiplier for the focussed image size.'),
    '#default_value' => $view->options['imageflow']['imageFocusM'],
  );
  $form['imageflow']['scrollbarP'] = array(
    '#type' => 'textfield',
    '#title' => t('Scrollbar width multiplier'),
    '#description' => t('Multiplier width of the scrollbar (0-1).'),
    '#default_value' => $view->options['imageflow']['scrollbarP'],
  );
  $form['imageflow']['imageFocusMax'] = array(
    '#type' => 'textfield',
    '#title' => t('Image focus max'),
    '#description' => t('Maximum number of images on each side of the focussed one.'),
    '#default_value' => $view->options['imageflow']['imageFocusMax'],
  );
}
