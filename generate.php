<?php
require('config.php');
require('tartan.inc');

$width = 280;
$stripe = 2;
$sett = array();

for($i=0; $i < sizeof($_POST['colors']); $i++) {
  // check for submitted blank values
  if ($_POST['colors'][$i] && $_POST['sizes'][$i]) {
    $sett[] = array('color' => $_POST['colors'][$i],
                    'count' => $_POST['sizes'][$i]);
  }
}
$name = stripslashes($_POST['name']);
$tartan = new LyzaTartan($name, $sett, 1);
$tartan->set_dynamic_scale($width);
$tartan->set_stripe_size($stripe);
if (isset($_POST['tartan_info'])) {
  $tartan->set_description(strip_tags(stripslashes($_POST['tartan_info']), '<a><p><strong><em><br><ul><li><ol><h1><h2><h3><h4><h5><h6>'));
}

$xml = $tartan->to_xml();

// Support jQuery Mobile automatic ajax "caching"
if (empty($name)) {
  $name = $_GET['name'];
}

$redirect_path = ($_POST['redirect_to_image'] === "true") ? 'image.php' : 'new_tartan.php';
header('Location:' . $redirect_path . '?name=' . $name . '&width=' . $width);
exit();

