<?php

/* class registration*/
spl_autoload_register(function ($v){
	$r = array(
		($v{0}=="c" ? "core" : "local"),
		($v{1}=="c" ? "cms" : "web"),
		mb_substr($v,2)
	);
	include "../class/{$r[0]}/{$r[1]}/{$r[2]}.inc";
});

/* config*/
$src[0]["cfg"] = new cwCfg("../conf/setup.json");

/* database */
$src[0]["dta"] = $src[0]["cfg"]->lc("Dta",$src[0]["cfg"]);

/* standard library */
$src[0]["std"] = $src[0]["cfg"]->lc("Std",$src[0]["dta"]);

/* variables */
$src[0]["var"] = $src[0]["cfg"]->lc("Var",$src[0]["std"]);

/* image cache */
$src[0]["imgc"] = $src[0]["cfg"]->lc("ImgC",$src[0]["var"]);

/* resources */
$src[0]["res"] = $src[0]["cfg"]->lc("Res",$src[0]["imgc"]);

/* user bumf */
$src[0]["usr"] = $src[0]["cfg"]->lc("Usr",$src[0]["std"]);

/* redirect bad ip addresses to the honeypot */
if($src[0]["usr"]->visitor()){
	if($src[0]["cfg"]->o["hnyFile"]!=""){
		if(mb_stristr($src[0]["std"]->o["uri"],$src[0]["cfg"]->o["hnyFile"])){
			include($src[0]["std"]->o["hp"]."/".$src[0]["cfg"]->o["hnyFile"]);
			die();
		}
		$hny = new cwHny($src[0]["cfg"]->o);
		$r = $hny->check($src[0]["std"]->o["rem"]);
		if($r['threat'] > 10 && $r['age'] < 15){
			$src[0]["std"]->header(array("loc"=>'/src/hp/'.$src[0]["cfg"]->o["hnyFile"]));
		}
	}
}

/* handle missing resource requests */
$src[0]["res"]->is($src[0]["std"]->o["cbc"]);

/* set the website timezone */
date_default_timezone_set($src[0]["var"]->fetch("timezone")[0]);

/* website language - try to  use the autolng based on the current domain, if not found try using the first breadcrumb */
foreach($src[0]["var"]->fetch("autolng") as $key=>$val){
	if($key==$src[0]["std"]->o["dom"]){
		$lng=$val[0];
		$ldir=$val[1]==1 ? "rtl":"ltr";
		break;
	}
	if($src[0]["std"]->o["bc"][0]==$val[0]){
		$lng=$val[0];
		$ldir=$val[1]==1 ? "rtl":"ltr";
		array_shift($src[0]["std"]->o["bc"]);
		if(!array_key_exists(0,$src[0]["std"]->o["bc"])){$src[0]["std"]->o["bc"][0]="";}
		$src[0]["std"]->o["cbc"] = end($src[0]["std"]->o["bc"]);
		break;		
	}
}

/* we have no configured language, so this is a good time to die */
if(!isset($lng)){die();}

/* is the requested page a part of the admin */
if($src[0]["std"]->o["bc"][0]=="src"){
	
	/* is the requested page for user login */
	if($src[0]["std"]->o["bc"][1]=="usr" && $src[0]["std"]->o["bc"][1]=="lgn"){
		
		if($src[0]["usr"]->visitor()){
			
		}
	}
	die();
}

/* page */
$src[0]["pge"]=$src[0]["cfg"]->lc("Pge",$src[0]["std"]);

/* lookup the breadcrumb */
$pge=$src[0]["pge"]->fetch(array("act"=>"id","dta"=>1));

/* set the headers */
$src[0]["std"]->header(array("ok"=>true));

include("../layouts/3.php");