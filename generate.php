<?php
require('config.php');
require('tartan.inc');

$sett = array();

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

$xml = $tartan->writeXML();
$tartan->setTargetWidth(160);
$tartan->writeImage();
$tartan->setTargetWidth(240);
$tartan->writeImage();
$tartan->writeHTML();
// Use $tartan->getBaseName() to get the base filename for a tartan.