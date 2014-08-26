<!DOCTYPE html>
<html class="<?=$ldir;?>" dir="<?=$ldir;?>" lang="<?=$lng;?>">

	<head>
		<meta charset="utf-8">
		<title>Testing the content</title>
		<link rel="stylesheet" href="<?=$src[0]["cfg"]->o["static"];?>/src/cwv/<?=$src[0]["var"]->mtf(array("typ"=>"css","dta"=>"cwv"));?>.css">
		<!---<script src="<?=$src[0]["cfg"]->o["static"];?>/src/cke/ckeditor.js"></script>-->
	</head>	
	<body>
		
		<div id="text-holder" contenteditable="true">
			<h1><?=$pge["title"][$lng];?></h1>
		</div>
		<div class="row">
			<div class="col all25 hgt20" style="background-color:#f00;"></div>
			<div class="col all25 hgt20" style="background-color:#0f0;"></div>
			<img width="100" height="100" src="<?=$src[0]["cfg"]->o["static"];?>/src/fiwv/100x100/100.jpg">
		</div>
		<script>
			/* CKEDITOR.inline(document.getElementById( 'text-holder' )); */
		</script>
	</body>

</html>