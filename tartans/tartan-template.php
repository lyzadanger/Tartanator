<!DOCTYPE html> 
<html> 
	<head> 
	<title><?php print $this->name ?>: The Tartanator</title> 
	
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
  <link rel="stylesheet" href="../css/jquery.mobile-1.0rc1.min.css" />
  <script src="../js/jquery-1.6.4.min.js"></script>
  <script src="../js/jquery.mobile-1.0rc1.min.js"></script>
</head> 
<body> 

<div data-role="page" id="<?php print $this->getBaseName(); ?>">
  <style type="text/css">
    #<?php print $this->getBaseName(); ?> { 
      background-image: url('<?php print PUBLIC_TARTAN_DIR ?>images/<?php print $this->getBaseName(); ?>-240.png'); 
      min-height: 240px;
    }
  </style>
	<div data-role="header" data-position="fixed">
    <a href="../tartans.html" data-rel="back" data-direction="reverse" data-icon="back" />Back</a>
		<h1><?php print $this->name; ?></h1>
	</div><!-- /header -->

	<div data-role="content">
    
	</div><!-- /content -->
	<div data-role="footer" data-position="fixed" data-theme="c">
		Bring forrit the tartan!
	</div><!-- /footer -->
</div><!-- /page -->

</body>
</html>