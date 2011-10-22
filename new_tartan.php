<!DOCTYPE html> 
<html> 
  <head> 
    <title>The Tartanator</title> 
  
    <meta name="viewport" content="width=device-width, initial-scale=1"> 

    <link rel="stylesheet" href="http://code.jquery.com/mobile/1.0b3/jquery.mobile-1.0b3.min.css" />
    <script type="text/javascript" src="http://code.jquery.com/jquery-1.6.3.min.js"></script>
    <script type="text/javascript" src="http://code.jquery.com/mobile/1.0b3/jquery.mobile-1.0b3.min.js"></script>
  </head>
  <body> 

  <div data-role="page" data-add-back-btn="true" data-back-btn-text="Create">

    <div data-role="header">
      <h2><?php echo $_GET['name'] ?></h2>
    </div><!-- /header -->

    <div data-role="content">  
      <img src="image.php?name=<?php echo $_GET['name'] ?>&amp;width=<?php echo $_GET['width'] ?>">
    </div><!-- /content -->

  </div><!-- /page -->

  </body>
</html>