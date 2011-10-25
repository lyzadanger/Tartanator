<?php
class LyzaTartan {
  public $name;
  
  private $sett;
  private $description;
  private $patternScale;
  private $stripe;
  
  private $warp;
  private $weft;
  private $weave;
  private $width;
  private $height;

  private $ready;
  
  function __construct($name, $sett=array(), $patternScale=1) {
    $this->name = $name;
    $this->setSett($sett);
    $this->setScale($patternScale, 2);
    $this->ready = false;
  }
  /**
   * Build the warp and weft and generate the weave 
   * (i.e. pixel matrices for image)
   * A call to this is needed whenever the object is dirtied
   * by changes to its attributes.
   */
  private function init() {
    $this->weft = array();
    $this->warp = array();
    $this->width = $this->height = 0;
    $this->weave = array();
    $this->buildPattern();
    $this->buildWeave();
    $this->ready = true;
  }
  
  /**
   * Populate tartan attributes from XML file
   */
  public function fromXml($filename) {
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
        $this->setPatternScale(1);
        $this->init();
        return TRUE;
      }
    }
    return FALSE;
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
  public function setDynamicScale($target_width) {
    $this->setPatternScale(1);
    if (!$this->ready) {
      $this->init();
    }
    $factor = $target_width / $this->width;
    $this->patternScale = $factor;
    $this->ready = false;
  }
  public function setDescription($description) {
    $this->description = $description;
  }
  public function getDescription() {
    return $this->description;
  }
  
  /**
   * Using the current state of the attributes, build the
   * warp and weft arrays to be used for generating a visual
   * representation of the pattern.
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
      $threads['color'] = $this->hex_to_rgb($threads['color']);
    }
    if (is_array($threads['color'])) {
      $my_color = $this->rgb_to_int($threads['color'][0], $threads['color'][1], $threads['color'][2]);
    }
    $my_chunk = array_fill(sizeof($this->warp), $my_count, $my_color);  
    return $my_chunk;
  }
  
  /**
   * This is sort of like a rasterizing method. Using the current
   * warp and weft, "weave" together a matrix of colors.
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
   * Build a PNG image and return it.
   *
   * @param int $width      Desired approximate pixel width
   * @return Image resource 
   */
  public function buildImage($width=NULL) {
    // TODO: Check to see if image file already exists
    if ($width) {
      $this->setDynamicScale($width);
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
          $channels = $this->int_to_rgb($rgb);
          $colors["color_" . $rgb] = imagecolorallocate($im, $channels[0], $channels[1], $channels[2]);
        }
        $ok = imagesetpixel($im, $x, $y, $colors["color_" . $rgb]);
      }
    }
    return $im;
  }
  
  /**
   * Build and deliver an image to the browser. Outputs!
   *
   * @param int $width    Approximate desired pixel width
   * @return NULL
   */
  public function showImage($width=NULL) {
    $im = $this->buildImage($width);
    header('Content-Type: image/png');
    imagepng($im);    
  }
  
  public function saveImage($width=NULL) {
    $im = $this->buildImage($width);
    // Filename: filepath + NEED TO DETERMINE NAME + whatever
  }
  
  /**
   * Generate a PNG from the current weave.
   */
  public function image($save=FALSE, $display=TRUE, $save_to=NULL) {
    $im = $this->buildImage();
    if ($save) {
      // Why yes, as written, this WOULD overwrite another tartan with 
      // the same name. 
      $save_to = ($save_to) ? $save_to : TARTAN_IMAGE_DIR . self::sanitize($this->name, TRUE) . '-' . $this->width . '.png';
      imagepng($im, $save_to);
      imagedestroy($im);
      if ($display) {
        $fp = fopen($save_to, 'rb');
        header('Content-Type: image/png');
        fpassthru($fp);
        exit;
      }
    }
    else if ($display) {
      header('Content-Type: image/png');
      imagepng($im);
    }
  }
  
  /**
   * Utility methods to convert colors
   */
  private function rgb_to_int($r, $g, $b, $a=NULL) {
    if (!$a) {
      return ($r << 16 | $g << 8 | $b);
    }
    else {
      return ($a << 24 | $r << 16 | $g << 8 | $b);
    }
  }
  // Don't try this with a 32-bit integer
  private function int_to_rgb($rgb) {
    $r = ($rgb >> 16) & 0xFF;
    $g = ($rgb >> 8) & 0xFF;
    $b = $rgb & 0xFF;
    return array($r, $g, $b);
  }
  private function hex_to_rgb($hex) {
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
  
  public static function toFilename($name) {
    
  }
  
  public static function fromFilename($name) {
  
  }
  
  public static function fromURLName($name) {
  
  }

  public static function sanitize($string = '', $is_filename = FALSE) {
    $string = urldecode(stripslashes($string));
    $string = preg_replace('/[^\w\-'. ($is_filename ? '~_\.' : ''). ']+/u', '-', $string);
    return mb_strtolower(preg_replace('/--+/u', '-', $string), 'UTF-8');
  }
  
  public function getNicename() {
    return self::sanitize($this->name, TRUE);
  }
  
  public function getFilename() {
    return TARTAN_XML_DIR . self::sanitize($this->name) . '.xml';
  }

  public function toXml($to_file=TRUE) {
    $tartan_xml = '';
    $tartan  = new SimpleXMLElement('<tartan />');
    $tartan->addAttribute('name', $this->name);
    $tartan->addAttribute('stripe', $this->stripe);
    $tartan->addAttribute('scale', 1);
    if ($this->getDescription()) {
      $tartan->addChild('description', $this->getDescription());
    }
    $stitches = $tartan->addChild('stitches');
    foreach ($this->sett as $settlet) {
      $stitch = $stitches->addChild('stitch');
      $stitch->addAttribute('color', $settlet['color']);
      $stitch->addAttribute('count', $settlet['count']);
    }
    $tartan_xml = $tartan->asXML();
    // Use a DOMDocument to format the XML
    $dom = new DOMDocument('1.0');
    $dom->preserveWhiteSpace = false;
    $dom->formatOutput = true; 
    $dom->loadXML($tartan_xml);

    if ($to_file) {
      if (!$this->name) {
        return false;
      }
      $fp = fopen(TARTAN_XML_DIR . self::sanitize($this->name, TRUE) . '.xml', 'w');
      if ($fp) {
        fwrite($fp, $dom->saveXML());
        fclose($fp);
      }
    }
    return $tartan_xml;
  }
}
