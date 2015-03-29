<?php

/**
 * @file
 * Work with images.
 */

/**
 * Exporting images.
 */
function style_manager_images_export($preset) {
  $out = array();
  if ($preset) {
    foreach ($preset as $item) {
      if (!empty($item['settings']['image']['url'])) {
        $base64 = _style_manager_get_base64_from_url($item['settings']['image']['url']);
        if ($base64) {
          $out[md5($item['settings']['image']['url'])] = $base64;
        }
      }
      if (!empty($item['settings']['css_code'])) {
        preg_match_all('/url\(\s*[\'"]?([^\'")]+)[\'"]?\s*\)/i', $item['settings']['css_code'], $matches, PREG_SET_ORDER);
        if ($matches) {
          foreach ($matches as $match_data) {
            if (!$match_data[1]) {
              continue;
            }
            $base64 = _style_manager_get_base64_from_url($match_data[1]);
            if ($base64) {
              $out[md5($match_data[1])] = $base64;
            }
          }
        }
      }
    }
  }
  return $out;
}


/**
 * Get file path return base64 data from file.
 * @see style_manager_images_export
 */
function _style_manager_get_base64_from_url($file_path) {
  $out = FALSE;
  $url = style_manager_get_rial_url($file_path);
  if (file_exists($url)) {
    $imgbinary = fread(fopen($url, "r"), filesize($url));
    $out = base64_encode($imgbinary);
  }
  return $out;
}


/**
 * Import images.
 */
function style_manager_images_import($preset, $images) {
  if ($preset) {
    foreach ($preset as &$item) {
      if (!empty($item['settings']['image']['url'])) {
        $item['settings']['image']['url'] = style_manager_images_import_file_save($item['settings']['image']['url'], $images);
      }
      if (!empty($item['settings']['css_code'])) {
        _style_manager_import_images_preg_callback(NULL, $images);
        $item['settings']['css_code'] = preg_replace_callback('/url\(\s*[\'"]?([^\'")]+)[\'"]?\s*\)/i', '_style_manager_import_images_preg_callback', $item['settings']['css_code']);
      }
    }
  }
  return $preset;
}


/**
 * Import images from from css widget.
 * @see: style_manager_images_import
 */
function _style_manager_import_images_preg_callback($matches, $images_arr = NULL) {
  $_images_arr = & drupal_static(__FUNCTION__);
  if (isset($images_arr)) {
    $_images_arr = $images_arr;
    return FALSE;
  }
  $new_url = FALSE;
  if (isset($matches[1])) {
    $new_url = style_manager_images_import_file_save($matches[1], $_images_arr);
    $new_url = "url('{$new_url}')";
  }
  if (!$new_url && isset($matches[0])) {
    return $matches[0];
  }
  else {
    return $new_url;
  }
}

/**
 * Save file.
 * @see style_manager_images_import
 */
function style_manager_images_import_file_save($url, $images) {
  $rial_path = style_manager_get_rial_url($url);
  if (file_exists($rial_path)) {
    return $url;
  }
  if (!empty($images[md5($url)])) {
    $img_hash = md5($images[md5($url)]);
    $img_data = base64_decode($images[md5($url)]);
    $pathinfo = pathinfo($url);
    $img_url = file_build_uri('style_manager_images/' . $img_hash . '.' . $pathinfo['extension']);
    if (file_exists($img_url)) {
      return file_create_url($img_url);
    }
    else {
      $file_uri = file_build_uri('style_manager_images');
      if (!file_prepare_directory($file_uri, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        return $url;
      }
    }
    file_unmanaged_save_data($img_data, $img_url, FILE_EXISTS_REPLACE);
    return file_create_url($img_url);
  }
}

/**
 * Get existing image or make a new color conversion.
 */
function style_manager_image_post_processing($settings) {
  module_load_include('inc', 'style_manager', 'includes/style_manager.render');
  $pathinfo = pathinfo($settings['url']);
  if (isset($pathinfo['extension']) && in_array($pathinfo['extension'], array('jpeg', 'jpg'))) {
    $extension = 'jpg';
  }
  else {
    $extension = 'png';
  }
  $new_url = file_build_uri('style_manager_cache/images/' . md5($settings['url']) . '-' . md5(serialize($settings)) . '.' . $extension);

  $success = file_exists($new_url) || style_manager_duotone_image($settings['url'], $new_url, $settings);
  if ($success) {
    return file_create_url($new_url);
  }
  else {
    return $settings['url'];
  }
}

/**
 * Change color of image.
 */
function style_manager_duotone_image($url_from, $url_to, $settings) {
  $url_from = urldecode($url_from);
  module_load_include('inc', 'style_manager', 'includes/style_manager.color');
  $pathinfo_from = pathinfo(strtolower($url_from));
  $pathinfo_to = pathinfo(strtolower($url_to));
  $url_from = style_manager_get_rial_url($url_from);
  if (!file_exists($url_from) || empty($pathinfo_from['extension'])) {
    return FALSE;
  }
  $alpha = FALSE;
  if (!file_prepare_directory($pathinfo_to['dirname'], FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
    return FALSE;
  }
  if ($pathinfo_from['extension'] == 'png') {
    $image = imagecreatefrompng($url_from);
    $alpha = TRUE;
  }
  if ($pathinfo_from['extension'] == 'jpg' || $pathinfo_from['extension'] == 'jpeg') {
    $image = imagecreatefromjpeg($url_from);
    $alpha = FALSE;
  }
  if ($pathinfo_from['extension'] == 'gif') {
    $image = imagecreatefromgif($url_from);
    $alpha = TRUE;
  }
  if (empty($image)) {
    return FALSE;
  }
  $new_image = style_manager_duotone_image_costum_mode($image, $settings, $alpha);
  if ($settings['contrast']) {
    if (!$new_image) {
      $new_image = $image;
    }
    imagefilter($new_image, IMG_FILTER_CONTRAST, $settings['contrast']);
  }
  elseif (!$new_image) {
    return FALSE;
  }

  $path_to = drupal_realpath($url_to);

  if ($pathinfo_to['extension'] == 'png') {
    return imagepng($new_image, $path_to, 2);
  }
  if ($pathinfo_to['extension'] == 'jpg' || $pathinfo_to['extension'] == 'jpeg') {
    return imagejpeg($new_image, $path_to, 95);
  }
}

/**
 * Prepare images for processing.
 */
function style_manager_duotone_image_costum_mode($image, $settings, $alpha) {
  $imagex = imagesx($image);
  $imagey = imagesy($image);
  $image2 = imagecreatetruecolor($imagex, $imagey);
  $image_overridden = FALSE;
  $color_store = array();
  if ($alpha) {
    imagesavealpha($image2, TRUE);
    imagealphablending($image2, FALSE);
    $transparent = imagecolorallocatealpha($image2, 255, 255, 255, 127);
    imagefilledrectangle($image2, 0, 0, $imagex, $imagey, $transparent);
    imagecopy($image2, $image, 0, 0, 0, 0, $imagex, $imagey);
  }
  else {
    imagecopy($image2, $image, 0, 0, 0, 0, $imagex, $imagey);
  }
  for ($x = 0; $x < $imagex; ++$x) {
    for ($y = 0; $y < $imagey; ++$y) {
      $imagecolorat = imagecolorat($image2, $x, $y);
      $image_color = imagecolorsforindex($image2, $imagecolorat);
      $image_color['alpha'] = empty($image_color['alpha']) ? 0 : $image_color['alpha'];
      $color_key = $image_color['red'] . $image_color['green'] . $image_color['blue'] . $image_color['alpha'];
      if (empty($color_store[$color_key])) {
        $hsl = style_manager_color_rgb2hsl($image_color['red'], $image_color['green'], $image_color['blue'], FALSE);
        $overriding_color = _style_manager_get_overriding_settings_for_color($settings['color_overriding'], $hsl);
        if (!$overriding_color && empty($settings['brightness']) && empty($settings['transparency'])) {
          $color_store[$color_key] = $imagecolorat;
        }
        else {
          $new_color_hsl = $hsl;
          if ($overriding_color) {
            $new_color_hsl = style_manager_color_convert($overriding_color['to_color']['hsl'], $hsl, $overriding_color['mode'], $settings['brightness']);
          }
          elseif ($settings['brightness']) {
            $new_color_hsl = style_manager_color_convert($hsl, $hsl, 'mode_h', $settings['brightness']);
          }
          $new_color_rgb = style_manager_color_hsl2rgb($new_color_hsl['h'], $new_color_hsl['s'], $new_color_hsl['l']);
          $new_color_rgb['alpha'] = style_manager_duotone_image_get_alpha($image_color['alpha'], $settings['transparency']);

          if ($settings['background_color'] && $new_color_rgb['alpha'] > 0) {
            $new_color_rgb = style_manager_duotone_image_set_bg_color($new_color_rgb, $settings['background_color']['rgb'], $new_color_rgb['alpha']);
            $new_color_rgb['alpha'] = 0;
          }
          if ($alpha) {
            $color_store[$color_key] = imagecolorallocatealpha($image2, $new_color_rgb['r'], $new_color_rgb['g'], $new_color_rgb['b'], $new_color_rgb['alpha']);
          }
          else {
            $color_store[$color_key] = imagecolorallocate($image2, $new_color_rgb['r'], $new_color_rgb['g'], $new_color_rgb['b']);
            $new_color_rgb['alpha'] = $image_color['alpha'];
          }
          if ($color_key != $new_color_rgb['r'] . $new_color_rgb['g'] . $new_color_rgb['b'] . $new_color_rgb['alpha']) {
            $image_overridden = TRUE;
          }
        }
      }
      imagesetpixel($image2, $x, $y, $color_store[$color_key]);
    }
  }
  if (!$image_overridden) {
    return FALSE;
  }
  return $image2;
}


/**
 * Add alpha to image.
 */
function style_manager_duotone_image_get_alpha($alpha, $correct) {
  $alpha += $correct;
  if ($alpha > 127) {
    $alpha = 127;
  }
  if ($alpha < 0) {
    $alpha = 0;
  }
  return $alpha;
}

/**
 * Add bg color to image.
 */
function style_manager_duotone_image_set_bg_color($new_color, $bg_color, $alpha) {
  $new_color['r'] = (int) ($new_color['r'] - ((($new_color['r'] - $bg_color['r']) / 127) * $alpha));
  $new_color['g'] = (int) ($new_color['g'] - ((($new_color['g'] - $bg_color['g']) / 127) * $alpha));
  $new_color['b'] = (int) ($new_color['b'] - ((($new_color['b'] - $bg_color['b']) / 127) * $alpha));
  return $new_color;
}
