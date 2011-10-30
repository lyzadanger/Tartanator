<?php
require_once('config.php');
require_once('inc/tartan.inc');

/**
 * Check some required form ($_POST) elements.
 * If they're all set and OK, return TRUE.
 *
 * @return Boolean
 */
function tartan_form_ready() {
  if (   is_array($_POST)
      && sizeof($_POST)
      && array_key_exists('colors', $_POST)
      && is_array($_POST['colors'])
      && array_key_exists('name', $_POST)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Instantiate and populate a LyzaTartan object
 * from form values.
 *
 * @return LyzaTartan
 */
function populate_tartan() {
  $sett = array();
  for($i=0; $i < sizeof($_POST['colors']); $i++) {
    // check for submitted blank values
    if ($_POST['colors'][$i] && $_POST['sizes'][$i]) {
      $sett[] = array('color' => $_POST['colors'][$i],
                      'count' => $_POST['sizes'][$i]);
    }
  }
  $name = stripslashes($_POST['name']);
  // Instantiate and populate a new LyzaTartan object from
  // the form values
  $tartan = new LyzaTartan($name);
  $tartan->setSett($sett);
  if (isset($_POST['tartan_info'])) {
    $tartan->setDescription(strip_tags(stripslashes($_POST['tartan_info'])));
  }
  return $tartan;
}

/**
 * Generate filesystem items for this tartan:
 * Image, HTML file, XML datafile.
 *
 * @param LyzaTartan $tartan    A populated LyzaTartan object
 */
function generate_tartan($tartan) {
  $xml = $tartan->writeXML();
  $tartan->setTargetWidth(200);
  $tartan->writeImage();
  $tartan->writeHTML();
}

if (tartan_form_ready()) {
  $tartan = populate_tartan();
  generate_tartan($tartan);
  if (array_key_exists('HTTP_X_REQUESTED_WITH', $_SERVER) && $_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest') {
    // jQuery Mobile sends this HTTP header when requesting resources via XHR (AJAX)
    echo $tartan->getPublicPath();
  } else {
    // For browsers that don't support JS/XHR
    // A full redirect to the newly-created Tartan HTML file
    header('Location:' . $tartan->getPublicPath());
    exit();
  }
}