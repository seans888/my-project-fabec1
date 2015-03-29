<?php // $Id: donate.admin.inc,v 1.1 2010/09/27 23:10:17 jerdavis Exp $

function donate_admin_overview() {
  module_load_include('admin.inc', 'pay', 'includes/pay');
  return pay_admin_pay_form_list('donate');
}
