/* $Id: donate.js,v 1.2 2010/09/30 21:35:55 jerdavis Exp $ */
/**
 * @file
 * jQuery functions for the donation form
 */
(function ($) {
Drupal.behaviors.donate = {
  attach: function (context) {
    $('.donate-other-amount').hide().siblings('label').hide();
    $('.donate-recommended-amount').click(function() {
      if ($('.donate-recommended-amount:checked').val() == 'other') {
        $('div[id*="other-1-wrapper"]').css('position', 'relative').css('top','-.5em').css('margin', '0');
        $('div[id*="other-wrapper"]').css('float','left').css('padding-right','.5em');
        $('.donate-other-amount').show().focus().siblings('label').hide();
      }
      else {
        $('div[id*="other-wrapper"]').css('float','none');
        $('.donate-other-amount').val('').hide().siblings('label').hide();
      }
    });
  }
}
})(jQuery);
