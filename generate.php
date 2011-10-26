<?php
require_once('config.php');
require_once('tartan.inc');

$post_ready = FALSE;
if (   is_array($_POST)
    && sizeof($_POST)
    && array_key_exists('colors', $_POST)
    && is_array($_POST['colors'])
    && array_key_exists('name', $_POST)) {
  $post_ready = TRUE;   
}

if ($post_ready) {
  $sett = array();
  
  /*
   * Form processing
   */
  for($i=0; $i < sizeof($_POST['colors']); $i++) {
    // check for submitted blank values
    if ($_POST['colors'][$i] && $_POST['sizes'][$i]) {
      $sett[] = array('color' => $_POST['colors'][$i],
                      'count' => $_POST['sizes'][$i]);
    }
  }
  $name = stripslashes($_POST['name']);
  $tartan = new LyzaTartan($name);
  
  $tartan->setSett($sett);
  if (isset($_POST['tartan_info'])) {
    $tartan->setDescription(strip_tags(stripslashes($_POST['tartan_info'])));
  }
  
  /*
   * Writing XML, images, HTML
   */
  $xml = $tartan->writeXML();
  $tartan->setTargetWidth(160);
  $tartan->writeImage();
  $tartan->setTargetWidth(240);
  $tartan->writeImage();
  $tartan->writeHTML();
  // Use $tartan->getBaseName() to get the base filename for a tartan.

  if ($_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest') {
    echo $tartan->getPublicPath();
  } else {
    header('Location:' . $tartan->getPublicPath());
    exit();
  }

}
