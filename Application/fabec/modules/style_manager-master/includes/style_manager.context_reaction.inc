<?php

/**
 * @file
 * Context integration.
 */

/**
 * Context reactions.
 */
class style_manager_reaction_preset extends context_reaction {
  /**
   * Provide a form element that allow the admin to chose a presets.
   */
  public function options_form($context) {
    $context_values = $this->fetch_from_context($context);
    $style_manager_cats = style_manager_get_cat_info();
    $form = array();
    foreach ($style_manager_cats as $cat) {
      if ($cat['info']['id'] != 'global_config') {
        $options = array(
          0 => t('Not override'),
          'disable' => t('Disabled'),
        );
      }
      foreach ($cat['preset_data'] as $key => $preset) {
        $options[$key] = t('Preset: @title', array('@title' => $preset['info']['preset_title']));
      }
      $form[$cat['info']['id']] = array(
        '#title' => check_plain($cat['info']['title']),
        '#options' => $options,
        '#type' => 'select',
        '#default_value' => isset($context_values[$cat['info']['id']]) ? $context_values[$cat['info']['id']] : 0,
      );
    }
    return $form;
  }

  /**
   * Override of options_form_submit().
   */
  public function options_form_submit($values) {
    return $values;
  }
}
