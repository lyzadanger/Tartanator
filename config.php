<?php
if (file_exists('config.inc')) {
  require_once('config.inc');
} else {
  // paths relative to DOCROOT
  define('TARTAN_DOC_ROOT', '/' .  substr(dirname(__FILE__), strlen($_SERVER['DOCUMENT_ROOT'])));
  define('PUBLIC_TARTAN_DIR', TARTAN_DOC_ROOT . '/tartans/');
  // Absolute filesystem paths
  define('TARTAN_XML_DIR', dirname($_SERVER['SCRIPT_FILENAME']) . '/tartans/data/');
  define('TARTAN_IMAGE_DIR', dirname($_SERVER['SCRIPT_FILENAME']) . '/tartans/images/');
  define('TARTAN_HTML_DIR', dirname($_SERVER['SCRIPT_FILENAME']) . '/tartans/');
}