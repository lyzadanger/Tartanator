<?php
if (file_exists('config.inc')) {
  require_once('config.inc');
} else {
  define('PUBLIC_TARTAN_DIR', '/tartans/');
  define('TARTAN_XML_DIR', dirname($_SERVER['SCRIPT_FILENAME']) . '/tartans/data/');
  define('TARTAN_IMAGE_DIR', dirname($_SERVER['SCRIPT_FILENAME']) . '/tartans/images/');
  define('TARTAN_HTML_DIR', dirname($_SERVER['SCRIPT_FILENAME']) . '/tartans/');
}