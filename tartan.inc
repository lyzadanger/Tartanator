<?php
class LyzaTartan {
  public $name;
  
  public  $sett;
  public  $description;
  public  $patternScale;
  public  $stripe;
  
  private $warp;
  private $weft;
  private $weave;
  private $width;
  private $targetWidth;
  private $height;
  private $XMLFilename;
  private $base;            // Filesystem-friendly name of tartan

  private $ready;
  
  function __construct($name='') {
    $this->name       = $name;
    $this->setPatternScale(1);    // Default pattern multiplier
    $this->setStripeSize(2);      // Default stripe size is 2
    $this->ready      = false;
  }
  /**
   * Build the warp and weft and generate the weave 
   * (i.e. pixel matrices for image)
   * A call to this is needed whenever the object is dirtied
   * by changes to its attributes.
   *
   * This method is very expensive and slow. It is only called when needed.
   */
  private function init() {
    $this->weft       = array();
    $this->warp       = array();
    $this->width      = $this->height = 0;
    $this->weave      = array();
    
    $this->buildPattern();
    $this->buildWeave();
    
    $this->ready      = true;
    $this->base       = $this->getBaseName();
  }
  
  /**
   * Populate tartan attributes from XML file
   */
  public function fromXML($base) {
    $this->setBase(self::sanitize($base));
    $filename = $this->getXMLFilename();
    if (file_exists($filename)) {
      $xml = simplexml_load_file($filename);
      if ($xml) {
        $this->name =  (string) $xml['name'];
        $sett = array();
        foreach($xml->stitches->children() as $stitch) {
          $sett[] = array('color' => (string) $stitch['color'],
                          'count' => (string) $stitch['count']);
        }
        $scale = (string) $xml['scale'];
        $stripe = (string) $xml['stripe'];
        $this->setSett($sett);
        $this->setStripeSize($stripe);
        $this->setPatternScale($scale);
        $this->XMLFilename = $filename;
        $this->ready = FALSE;
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Using the current state of the attributes, build the
   * warp and weft arrays to be used for generating a visual
   * representation of the pattern.
   *
   * Result: Warp and weft arrays from which an image
   * can be "woven"
   */
  private function buildPattern() {
    $this->warp = array();
    $this->weft = array();
    $count = 0;
    for($i = 0; $i < sizeof($this->sett); $i++) { // Out
      $threads = $this->sett[$i];
      $this->warp = array_merge($this->warp, $this->buildChunk($threads));
    }
    for($i = sizeof($this->sett) - 1; $i >= 0; $i--) { // Back
      // Pivots would matter HERE
      if ($i == sizeof($this->sett) - 1) {
        continue;
      }
      else if ($i == 0) {
        continue;
      }
      $threads = $this->sett[$i];
      $this->warp = array_merge($this->warp, $this->buildChunk($threads));
    }
    $this->weft = $this->warp;
    $this->width = $this->height = sizeof($this->weft);
  }
  
  /**
   * Utility method to build chunks (subsections) of the pattern
   * for the warp and the weft.
   */
  private function buildChunk($threads) {
    $my_count = $threads['count'] * $this->patternScale;
    $my_color = 0;
    if (is_string($threads['color']) && strpos($threads['color'], '#') !== FALSE) {
      $threads['color'] = self::hexToRGB($threads['color']);
    }
    if (is_array($threads['color'])) {
      $my_color = self::RGBToInt($threads['color'][0], $threads['color'][1], $threads['color'][2]);
    }
    $my_chunk = array_fill(sizeof($this->warp), $my_count, $my_color);  
    return $my_chunk;
  }
  
  /**
   * This is sort of like a rasterizing method. Using the current
   * warp and weft, "weave" together a matrix of colors.
   *
   * Result: We're ready to generate a raster image, when needed
   */
  private function buildWeave() {
    $this->weave = array();
    $s = $this->stripe;
    $warp       = true;
    $push       = 1;
    $push_state = true;
    $state      = true;
    for ($x = 0; $x < $this->width; $x++) {
      $state = $push_state;
      $count = $push;
      for ($y = 0; $y < $this->height; $y++) {
        if ($state) {
          $this->weave[$x][$y] = $this->warp[$x];
        }
        else {
          $this->weave[$x][$y] = $this->warp[$y];
        }
        $count++;
        if ($count > $s) {
          $count = 1;
          $state = !$state;
        }
      } // End 'pixel' (or stitch)
      $push++;
      if ($push > $s) {
        $push = 1;
        $push_state = !$push_state;
      }
    } // End row
  }
  
  /**
   * Build an image  and RETURN it. At this point
   * we could output a PNG, GIF, JPG, whatever. Right
   * now I'm doing PNGs, but there's no reason one couldn't
   * do other image formats.
   *
   * @param int $width      Desired approximate pixel width
   * @return Image resource 
   */
  private function buildImage($width=NULL) {
    if ($width) {
      $this->setTargetWidth($width);
    }
    if (!$this->ready) {
      $this->init();
    }
    // Create an image resource
    $im = imagecreate($this->width, $this->height);
    // The first allocated color will define the background color
    $black = imagecolorallocate($im, 0,0,0);
    $colors = array();  // An array to hold allocated colors
    for($x = 0; $x < $this->width; $x++) {
      for ($y = 0; $y < $this->height; $y++) {
        $rgb = $this->weave[$x][$y];
        if (!array_key_exists("color_" . $rgb, $colors)) {
          $channels = self::intToRGB($rgb);
          $colors["color_" . $rgb] = imagecolorallocate($im, $channels[0], $channels[1], $channels[2]);
        }
        $ok = imagesetpixel($im, $x, $y, $colors["color_" . $rgb]);
      }
    }
    return $im;
  }
    
  /*
   * Generate and display an image
   */
  public function toImage($cache=TRUE) {
    if ($cache) {
      $this->writeImage();
    }
    $img_file = $this->getImageFilename();
    if (file_exists($img_file)) {
      $fp = fopen($img_file, 'rb');
      header('Content-Type: image/png');
      fpassthru($fp);
      exit;
    }
    else if (!$cache) {
      $im = $this->buildImage();
      header('Content-Type: image/png');
      imagepng($im);
    }
  }

  /*
   * Generate and cache image of tartan at present settings
   */
  public function writeImage($force_recreate=FALSE) {

    $img_file = $this->getImageFilename();
    if (file_exists($img_file) && !$force_recreate) {
      return TRUE;
    }
    $im = $this->buildImage();
    $success = imagepng($im, $img_file);
    imagedestroy($im);
    return $success;
    
  }
  
  /**
   * Write an HTML file to the filesystem for this
   * tartan. Use a template.
   */
  public function writeHTML($force_recreate=FALSE) {
    $filename = $this->getHTMLFilename();
    ob_start();
    include(TARTAN_HTML_DIR . 'tartan-template.php');
    $string = ob_get_clean();
    $fp = fopen($filename, 'w+');
    if ($fp) {
      fwrite($fp, $string);
      fclose($fp);
    }
  }
  
  public static function sanitize($string = '') {
    $is_filename = TRUE;
    $string = urldecode(stripslashes($string));
    $string = preg_replace('/[^\w\-'. ($is_filename ? '~_\.' : ''). ']+/u', '-', $string);
    return mb_strtolower(preg_replace('/--+/u', '-', $string), 'UTF-8');
  }
  
  /*
   * Return full path of HTML file for this tartan.
   * Intended to be used to write HTML files to that path.
   *
   * @return string filename      Full path
   */
  public function getHTMLFilename() {
    $filename = sprintf('%s%s.html', TARTAN_HTML_DIR, $this->getBaseName());
    return $filename;
  }

  public function getPublicPath() {
    $filename = sprintf('%s%s.html', PUBLIC_TARTAN_DIR, $this->getBaseName());
    return $filename;
  }
  
  /*
   * Return full path to where an image should either exist or be written
   * to for this tartan.
   *
   * @param string $img_type      Currently this only supports 'PNG'
   *
   * @return string              Full path
   */
  public function getImageFilename($img_type='PNG') {
    $width = ($this->targetWidth) ? $this->targetWidth : $this->width;
    $extension = '';
    switch($img_type) {
      case 'PNG':
      default:
        $extension = 'png';
    }
    $filename = sprintf("%s%s-%s.%s", TARTAN_IMAGE_DIR, $this->getBaseName(), $width, $extension);
    return $filename;
  }
  
  /**
   * Return full path to where the XML file is or should be.
   * 
   * @return string             Full path
   */
  public function getXMLFilename() {
    $filename = sprintf('%s%s.xml', TARTAN_XML_DIR, $this->getBaseName());
    return $filename;
  }

  /*
   * This is a bit complex, but in a nutshell, this should
   * return a string of the filesystem-safe "filename" (no path or extension)
   * to use for tartan-related resources (images, xml, html)
   * 
   */
  public function getBaseName($test_exists=TRUE) {
    if ($this->base) {
      return $this->base;
    }
    $base = self::sanitize($this->name);
    if ($test_exists) {
      $candidate = TARTAN_XML_DIR . $base . '.xml';
      $candidate_base = $base;
      $tries = 0;
      while (file_exists($candidate) && $tries < 50) {
        $tries++;
        $candidate_base = $base . '-' . (string) $tries;
        $candidate = sprintf("%s%s.xml", TARTAN_XML_DIR, $candidate_base);
      }
      $this->base = $candidate_base;
      return $candidate_base;  
    }
    else {
      $this->setBase($base);
      return $base;
    }
  }

  /*
   * Generate XML (via toXML()) and write to file.
   */
  public function writeXML() {
    $tartan_xml = $this->toXML();
    // Use a DOMDocument to format the XML
    $dom = new DOMDocument('1.0');
    $dom->preserveWhiteSpace = false;
    $dom->formatOutput = true; 
    $dom->loadXML($tartan_xml);

      if (!$this->name) {
        return false;
      }
      $filename = $this->getXMLFilename();
      $fp = fopen($filename, 'w');
      if ($fp) {
        fwrite($fp, $dom->saveXML());
        fclose($fp);
      }
      $this->XMLFilename = $filename;
  }

  /*
   * Generate XML string representing tartan pattern.
   */
  public function toXml() {
    $tartan_xml = '';
    $tartan  = new SimpleXMLElement('<tartan />');
    $tartan->addAttribute('name', $this->name);
    $tartan->addAttribute('stripe', $this->stripe);
    $tartan->addAttribute('scale', 1);
    if ($this->description) {
      $tartan->addChild('description', $this->description);
    }
    $stitches = $tartan->addChild('stitches');
    foreach ($this->sett as $settlet) {
      $stitch = $stitches->addChild('stitch');
      $stitch->addAttribute('color', $settlet['color']);
      $stitch->addAttribute('count', $settlet['count']);
    }
    $tartan_xml = $tartan->asXML();
    return $tartan_xml;
  }
  
  /**
   * How many total stitches are in this pattern?
   * Look at the 'count' element of each piece of the
   * sett. Then double that, except for the first and
   * last stitches (emulating how tartans are woven).
   */
  public function getStitchCount() {
    if (!$this->sett || !is_array($this->sett)) {
      return 0;
    }
    $total = 0;
    foreach ($this->sett as $index => $stitch) {
      $total += $stitch['count'];
      if ($index != 0 && $index != sizeof($this->sett) - 1) {
        // Account for pivots
        $total += $stitch['count'];
      }
    }
    return $total;
  }
  
  /**
   * Setters
   */
  public function setSett($sett) {
    $this->sett = $sett;
    $this->ready = false;
  }
  public function setPatternScale($scale) {
    $this->patternScale = $scale;
    $this->ready = false;
  }
  public function setStripeSize($size) {
    $this->stripe = $size;
    $this->ready = false;
  }
  public function setScale($pattern, $sett=2) {
    $this->setPatternScale($pattern);
    $this->setStripeSize($sett);
    $this->ready = false;
  }
  public function setTargetWidth($target_width) {
    $stitches = $this->getStitchCount();
    $stitches = ($stitches) ? $stitches : 1;
    $factor = $target_width / $stitches;
    $this->targetWidth = $target_width;
    $this->setPatternScale($factor);
    $this->ready = false;
  }
  public function setDescription($description) {
    $this->description = $description;
  }
  private function setBase($base) {
    $this->base = $base;
  }
  /**
   * I don't anticipate this needing to happen, but
   * thought I'd leave an escape valve. Once a base
   * has been determined for a given tartan, it shouldn't
   * really change.
   */
  private function clearBase() {
    $this->base = NULL;
  }

  /**
   * Utility methods to convert colors
   */
  public static function RGBToInt($r, $g, $b, $a=NULL) {
    if (!$a) {
      return ($r << 16 | $g << 8 | $b);
    }
    else {
      return ($a << 24 | $r << 16 | $g << 8 | $b);
    }
  }
  // Don't try this with a 32-bit integer
  public static function intToRGB($rgb) {
    $r = ($rgb >> 16) & 0xFF;
    $g = ($rgb >> 8) & 0xFF;
    $b = $rgb & 0xFF;
    return array($r, $g, $b);
  }
  public static function hexToRGB($hex) {
    $hex = preg_replace("/[^0-9A-Fa-f]/", '', $hex); 
    $rgbArray = array(); 
    if (strlen($hex) == 6) { //If a proper hex code, convert using bitwise operation. No overhead... faster
      $colorVal = hexdec($hex);
      $rgbArray[] = 0xFF & ($colorVal >> 0x10);
      $rgbArray[] = 0xFF & ($colorVal >> 0x8);
      $rgbArray[] = 0xFF & $colorVal;
    }
    return $rgbArray;
  }

}
