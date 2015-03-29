<?php

class donate extends pay_form {

  var $menu_path;
  var $header_text = array('value' => NULL);
  var $other_amount = TRUE;
  var $recommended_amounts;
  var $footer_text = array('value' => NULL);
  var $confirmation_text = array('value' => 'Thank you for your contribution.');
  var $confirmation_path = '';
  var $embeddable = TRUE;

  function __construct($values) {
    parent::__construct($values);
    $default_format = filter_default_format();
    $this->header_text['format'] = isset($this->header_text['format']) ? $this->header_text['format'] : $default_format;
    $this->footer_text['format'] = isset($this->footer_text['format']) ? $this->footer_text['format'] : $default_format;
    $this->confirmation_text['format'] = isset($this->confirmation_text['format']) ? $this->confirmation_text['format'] : $default_format;
  }

  /**
   * Add a set_amount function to handle the amount/other amount/etc total.
   */
  function set_amount($value = NULL) {
    $this->amount = (float) $value;
  }

  function set_amount_other($value = NULL) {
    $this->amount_other = (float) $value;
  }

  /**
   * All donation forms are embeddable by default.
   */
  function set_embeddable($value = NULL) {
    if (is_null($value)) {
      $this->embeddable = TRUE;
    }
    else {
      $this->embeddable = (bool) $value;
    }
  }

  function recommended_amounts() {
    $amounts = array();
    if ($this->recommended_amounts) {
      foreach (explode("\n", $this->recommended_amounts) as $item) {
        list($key, $val) = explode('|', $item);
        if (!$val) $val = $key;
        $amounts[(float)trim(filter_xss($key))] = trim(filter_xss($val));
      }
    }
    return $amounts;
  }

  function settings_form(&$form, &$form_state) {
    parent::settings_form($form, $form_state);

    $group = $this->handler();
    $form[$group]['donate'] = array(
      '#type' => 'fieldset',
      '#title' => t('Donation settings'),
      '#tree' => FALSE,
    );

    $form[$group]['donate']['menu_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Menu path'),
      '#default_value' => $this->menu_path,
    );

    $form[$group]['donate']['header']['header_text'] = array(
      '#type' => 'text_format',
      '#base_type' => 'textarea',
      '#format' => $this->header_text['format'],
      '#title' => t('Header text'),
      '#description' => t('Some text that will appear before the payment information'),
      '#default_value' => $this->header_text['value'],
    );

    $form[$group]['donate']['recommended_amounts'] = array(
      '#type' => 'textarea',
      '#title' => t('Recommended amounts'),
      '#default_value' => $this->recommended_amounts,
    );

    $form[$group]['donate']['other_amount'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow visitors to choose an amount'),
      '#description' => t('Add a freeform "Other amount" field that contributors can populate.'),
      '#default_value' => $this->other_amount,
    );

    $form[$group]['donate']['total_goal'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Total goal'),
      '#description' => t('The goal amount for this donation form.  This is optional and useful for tracking purposes.'),
      '#default_value' => $this->total_goal,
    );

    $form[$group]['donate']['footer']['footer_text'] = array(
      '#type' => 'text_format',
      '#base_type' => 'textarea',
      '#format' => $this->footer_text['format'],
      '#title' => t('Footer text'),
      '#description' => t('Some text that will appear after the payment information.'),
      '#default_value' => $this->footer_text['value'],
    );

    $form[$group]['donate']['confirmation']['confirmation_text'] = array(
      '#type' => 'text_format',
      '#base_type' => 'textarea',
      '#format' => $this->confirmation_text['format'],
      '#title' => t('Confirmation text'),
      '#description' => t('Assuming that everything went well, provide some "thank you" or other confirmation text.  If you enter a redirect URL, this message will appear as a standard drupal message'),
      '#default_value' => $this->confirmation_text['value'],
    );

    $form[$group]['donate']['confirmation_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Confirmation path'),
      '#default_value' => $this->confirmation_path,
      '#description' => t('Enter a drupal path or a fully-qualified URL.  If you omit this, the confirmation message, above, will be displayed.'),
    );
  }

  function settings_form_validate($form, &$form_state) {
    parent::settings_form_validate($form, $form_state);
  }

  function settings_form_submit($form, &$form_state) {
    // Return to the list of donation forms after submitting settings.
    $form_state['redirect'] = 'admin/structure/donate';
    
    parent::settings_form_submit($form, $form_state);

    // Do a menu rebuild to pick up any new menu paths.
    menu_rebuild();
  }

  function form(&$form, &$form_state) {
    $group = $this->handler();

    // "Other amount" capacity is always disabled for amount-only forms.
    if ($form_state['pay_form_type'] == 'amount') {
      $this->other_amount = FALSE;
    }

    // If an amount-only form was submitted, get the amount from $_GET.
    if (isset($_GET[$group]['amount'])) {
      $this->set_amount($_GET[$group]['amount']);
    }

    drupal_add_js(drupal_get_path('module','donate') .'/js/donate.js');

    if ($this->header_text) {
      $form[$group]['header_text'] = array(
        '#type' => 'markup',
        '#markup' => check_markup($this->header_text['value'], $this->header_text['format']),
      );
    }

    if ($amounts = $this->recommended_amounts()) {
      if ($this->other_amount) {
        $amounts['other'] = t('Other amount');
      }
      $form[$group]['amount'] = array(
        '#name' => 'amount',
        '#type' => 'radios',
        '#options' => $amounts,
        '#title' => t('Donation amount'),
        '#default_value' => isset($this->amount) ? $this->amount : '',
        '#attributes' => array('class' => array('donate-recommended-amount')),
        '#prefix' => '<div class="donate-recommended">',
        '#suffix' => '</div>'
      );

      if ($this->other_amount) {
        $form[$group]['amount_other'] = array(
          '#type' => 'textfield',
          '#title' => t('Other amount'),
          '#size' => 10,
          '#attributes' => array('class' => array('donate-other-amount')),
          '#default_value' => isset($this->amount_other) ? $this->amount_other : '',
        );
      }

      // Omit radio buttons if there's only 1 amount to select from.
      if (count($amounts) == 1) {
        $form[$group]['amount']['#access'] = FALSE;
        $form[$group]['amount']['#default_value'] = array_keys($amounts);
        $form[$group]['amount_display'] = array(
          '#type' => 'markup',
          '#value' => $label,
          '#attributes' => array('class' => array('donate-amount')),
        );
      }
    }
    else {
      $form[$group]['amount_other'] = array(
        '#type' => 'textfield',
        '#title' => t('Donation amount'),
        '#size' => 10,
      );
    }

    // The basic payment details from the parent form.
    parent::form($form, $form_state);

    // Ensure there is a "total" field, but populate it on the validate hook.
    $form[$group]['total'] = array(
      '#type' => 'textfield',
      '#access' => FALSE,
    );

    $form[$group]['notes']['#title'] = $this->notes_title;
    $form[$group]['notes']['#description'] = $this->notes_description;

    if ($this->footer_text) {
      $form[$group]['footer_text'] = array(
        '#type' => 'markup',
        '#markup' => check_markup($this->footer_text['value'], $this->footer_text['format']),
      );
    }

    // There's a confirmation message.  Display the message instead of the form.
    if (isset($form_state['storage']['confirmation'])) {
      $form[$group] = array(
        '#type' => 'markup',
        '#markup' => check_markup($this->confirmation_text['value'], $this->confirmation_text['format']),
      );

      // Remove submit button on confirmation page.
      $form['submit']['#access'] = FALSE;
    }
  }

  function form_validate($form, &$form_state) {

    // Use the "other amount" value if it's supplied.
    $values = $this->form_values($form_state);
    if (!empty($values['amount_other'])) {
      $total = $values['amount_other'];
    }

    // Alternatively, use the amount selected value from radio buttons.
    elseif($values['amount']) {
      $total = $values['amount'];
    }

    // Remove any non-numeric characters before classing as a float.
    $total = preg_replace('/[^\d\.]/', '', $total);

    // Set the form value so our parent thinks it was there all along.
    form_set_value($form[$this->handler()]['total'], (float)$total, $form_state);

    // Call the parent validator to handle amounts and other inputs.
    parent::form_validate($form, $form_state);
  }

  function form_submit($form, &$form_state) {
    parent::form_submit($form, $form_state);

    if (!isset($form_state['rebuild'])) {
      if (!isset($form_state['redirect'])) {
        if ($this->confirmation_path) {
          $form_state['redirect'] = $this->confirmation_path;
        }
        else {
          $form_state['storage']['confirmation'] = TRUE;
          $form_state['rebuild'] = TRUE;
        }
      }
    }
  }
}
