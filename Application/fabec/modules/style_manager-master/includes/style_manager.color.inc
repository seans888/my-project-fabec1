<?php

/**
 * @file
 * Convert color model.
 */

/**
 * Convert hexadcimal value to RGB(r, g, b).
 */
function style_manager_color_hex2rgb($hex) {
  $hex = style_manager_color_check_hex($hex);
  $dec = hexdec($hex);
  $rgb = array(
    "r" => (0xFF & ($dec >> 0x10)),
    "g" => (0xFF & ($dec >> 0x8)),
    "b" => (0xFF & $dec),
  );
  return $rgb;
}

/**
 * Convert hexadcimal value to HSV(h, s, v).
 */
function style_manager_color_hex2hsv($hex) {
  list($r, $g, $b) = array_values(style_manager_color_hex2rgb($hex));
  return style_manager_color_rgb2hsv($r, $g, $b);
}

/**
 * Convert hexadcimal value to HSL(h, s, l).
 */
function style_manager_color_hex2hsl($hex) {
  list($r, $g, $b) = array_values(style_manager_color_hex2rgb($hex));
  return style_manager_color_rgb2hsl($r, $g, $b);
}

/**
 * Convert RGB(r, g, b) to hexadecimal value.
 */
function style_manager_color_rgb2hex($r, $g, $b) {
  $hex = "";
  for ($i = 0; $i < 3; $i++) {
    switch ($i) {
      case 0:
        $d = dechex($r);
        break;

      case 1:
        $d = dechex($g);
        break;

      case 2:
        $d = dechex($b);
        break;

    }
    if (strlen($d) == 1) {
      $d = "0" . $d;
    }
    $hex .= $d;
  }
  return strtoupper($hex);
}

/**
 * Convert RGB(r, g, b) to HSV(h, s, v).
 */
function style_manager_color_rgb2hsv($r, $g, $b) {
  $r = ($r / 255);
  $g = ($g / 255);
  $b = ($b / 255);
  $min = min($r, $g, $b);
  $max = max($r, $g, $b);

  $h = style_manager_color_parse_hue($r, $g, $b);
  $s = ($max == 0) ? 0 : round((1 - ($min / $max)) * 100);
  $v = round($max * 100);

  return array("h" => $h, "s" => $s, "v" => $v);
}

/**
 * Convert RGB(r, g, b) to HSL(h, s, l).
 */
function style_manager_color_rgb2hsl($r, $g, $b, $round = TRUE) {
  $r = ($r / 255);
  $g = ($g / 255);
  $b = ($b / 255);
  $min = min($r, $g, $b);
  $max = max($r, $g, $b);

  $h = style_manager_color_parse_hue($r, $g, $b, $round);
  $l = (0.5 * ($max + $min));

  if ($max == $min) {
    $s = 0;
  }
  else {
    if ($l <= 0.5) {
      $s = (($max - $min) / (2 * $l));
    }
    else {
      $s = (($max - $min) / (2 - (2 * $l)));
    }
  }

  $s = $s * 100;
  $l = $l * 100;
  if ($round) {
    $s = round($s);
    $l = round($l);
  }

  return array("h" => $h, "s" => $s, "l" => $l);
}

/**
 * Convert HSV(h, s, v) to hexadecimal value.
 */
function style_manager_color_hsv2hex($h, $s, $v) {
  list($r, $g, $b) = array_values(style_manager_color_hsv2rgb($h, $s, $v));
  return style_manager_color_rgb2hex($r, $g, $b);
}

/**
 * Convert HSV(h, s, v) to RGB(r, g, b).
 */
function style_manager_color_hsv2rgb($h, $s, $v) {
  $s /= 100;
  $v /= 100;

  $hm = (floor($h / 60) % 6);
  $f = (($h / 60) - floor($h / 60));
  $p = ($v * (1 - $s));
  $q = ($v * (1 - $f * $s));
  $t = ($v * (1 - (1 - $f) * $s));

  switch ($hm) {
    case 0:
      $rgb = array($v, $t, $p);
      break;

    case 1:
      $rgb = array($q, $v, $p);
      break;

    case 2:
      $rgb = array($p, $v, $t);
      break;

    case 3:
      $rgb = array($p, $q, $v);
      break;

    case 4:
      $rgb = array($t, $p, $v);
      break;

    case 5:
      $rgb = array($v, $p, $q);
      break;

  }

  list($r, $g, $b) = $rgb;
  $r = round(255 * $r);
  $g = round(255 * $g);
  $b = round(255 * $b);
  return array("r" => $r, "g" => $g, "b" => $b);
}

/**
 * Convert HSV(h, s, v) to HSL(h, s, l).
 */
function style_manager_color_hsv2hsl($h, $s, $v) {
  list($r, $g, $b) = array_values(style_manager_color_hsv2rgb($h, $s, $v));
  return style_manager_color_rgb2hsl($r, $g, $b);
}

/**
 * Convert HSL(h, s, l) to hexadecimal value.
 */
function style_manager_color_hsl2hex($h, $s, $l) {
  list($r, $g, $b) = array_values(style_manager_color_hsl2rgb($h, $s, $l));
  return style_manager_color_rgb2hex($r, $g, $b);
}

/**
 * Convert HSL(h, s, l) to RGB(r, g, b).
 */
function style_manager_color_hsl2rgb($h, $s, $l) {
  $h /= 360;
  $s /= 100;
  $l /= 100;

  $q = ($l < 0.5) ? ($l * (1 + $s)) : ($l + $s - ($l * $s));
  $p = ((2 * $l) - $q);

  $rgb = array();
  for ($i = 0; $i < 3; $i++) {
    switch ($i) {
      case 0:
        $t = ($h + (1 / 3));
        break;

      case 1:
        $t = $h;
        break;

      case 2:
        $t = ($h - (1 / 3));
        break;

    }

    if ($t < 0) {
      $t += 1.0;
    }
    if ($t > 1) {
      $t -= 1.0;
    }

    if ($t < (1 / 6)) {
      $rgb[] = ($p + (($q - $p) * 6 * $t));
    }
    elseif (((1 / 6) <= $t) && ($t < 0.5)) {
      $rgb[] = $q;
    }
    elseif ((0.5 <= $t) && ($t < (2 / 3))) {
      $rgb[] = ($p + (($q - $p) * 6 * ((2 / 3) - $t)));
    }
    else {
      $rgb[] = $p;
    }
  }

  list($r, $g, $b) = $rgb;
  $r = round(255 * $r);
  $g = round(255 * $g);
  $b = round(255 * $b);
  return array("r" => $r, "g" => $g, "b" => $b);
}

/**
 * Convert HSL(h, s, l) to HSV(h, s, v).
 */
function style_manager_color_hsl2hsv($h, $s, $l) {
  list($r, $g, $b) = array_values(style_manager_color_hsl2rgb($h, $s, $l));
  return style_manager_color_rgb2hsv($r, $g, $b);
}

/**
 * Parses the hue degree used in converting ro both HSV and HSL values.
 */
function style_manager_color_parse_hue($r, $g, $b, $round = TRUE) {
  $min = min($r, $g, $b);
  $max = max($r, $g, $b);
  if ($max == $min) {
    $h = 0;
  }
  else {
    if (($max == $r) && ($g >= $b)) {
      $h = (60 * (($g - $b) / ($max - $min)));
    }
    else {
      if (($max == $r) && ($g < $b)) {
        $h = (60 * (($g - $b) / ($max - $min)) + 360);
      }
      else {
        if ($max == $g) {
          $h = (60 * (($b - $r) / ($max - $min)) + 120);
        }
        else {
          $h = (60 * (($r - $g) / ($max - $min)) + 240);
        }
      }
    }
  }
  if ($round) {
    return round($h);
  }
  else {
    return $h;
  }
}


/**
 * Checks the hexadecimal string for illegal characters.
 */
function style_manager_color_check_hex($hex) {
  $phex = preg_replace("/[^a-fA-F0-9]/", "", $hex);
  if (strlen($phex) == 6) {
    return $phex;
  }
  elseif (strlen($phex) == 3) {
    return $phex[0] . $phex[0] . $phex[1] . $phex[1] . $phex[2] . $phex[2];
  }
  else {
    return FALSE;
  }
}


/**
 * Color convert.
 */
function style_manager_color_convert($to_hsl, $from_hsl, $mode, $l_correct = FALSE) {
  $from_hsl['h'] = $to_hsl['h'];
  if ($mode == 'mode_hs' || $mode == 'mode_hsl') {
    $from_hsl['s'] = $to_hsl['s'];
  }
  if ($mode == 'mode_hl' || $mode == 'mode_hsl') {
    $from_hsl['l'] += ($to_hsl['l'] - 50);
  }
  if ($l_correct) {
    $from_hsl['l'] += $l_correct;
  }
  if ($from_hsl['l'] < 0) {
    $from_hsl['l'] = 0;
  }
  if ($from_hsl['l'] > 100) {
    $from_hsl['l'] = 100;
  }
  return $from_hsl;
}
