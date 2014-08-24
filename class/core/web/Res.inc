<?php

class cwRes {
	
	public function __construct($ImgC){
		$this->ImgC=$ImgC;
	}
	
	public function is($v){
		$r=array("css","js","jpg","png");
		if(!in_array(pathinfo($v,PATHINFO_EXTENSION),$r) || $this->ImgC->Var->Std->o["bc"][0]!="src" || count($this->ImgC->Var->Std->o["bc"])<3){
			return false;
		}
		switch($this->ImgC->Var->Std->o["bc"][1]){
			case "fiwv":
				if(count($this->ImgC->Var->Std->o["bc"])==4){
					if(is_dir($this->ImgC->Var->Std->o["fiwv"]."/".$this->ImgC->Var->Std->o["bc"][2])){
						if(is_file(str_replace(array("jpg","png"),"webp",$this->ImgC->Var->Std->o["fiwv"]."/".$v))){
							/* we need to crunch the image then serve it */
							/* TODO */
							$this->ImgC->Var->Std->o["fiwv"]."/".$this->ImgC->Var->Std->o["bc"][2]."/".$v;
							$this->img(array(
								"act"=>"create",
								"dta"=>array(
									"src"=>$this->ImgC->Var->Std->o["fiwv"],
									"ic"=>$this->ImgC->Var->Std->o["bc"][2],
									"nme"=>pathinfo($v,PATHINFO_FILENAME),
									"ext"=>pathinfo($v,PATHINFO_EXTENSION)
							)));
						}
					}
				}
				die();
			case "ccv":
			case "cwv":
				$this->ImgC->Var->compress(array("typ"=>pathinfo($v,PATHINFO_EXTENSION),"dta"=>$this->ImgC->Var->Std->o["bc"][1]));
				die();
		}
	}
	
	
	
	public function img($v){
		switch($v["act"]){
			case "orientation":
				$r=getimagesize($v);
				if($r[0]>$r[1]){
					return "x";
				} elseif($r[0]<$r[1]){
					return "y";
				} else {return "s";}
			case "dwebp":
				system("dwebp {$v["dta"]["src"]}/{$v["dta"]["nme"]}.webp -o {$v["dta"]["src"]}/{$v["dta"]["nme"]}.png");
				if(is_file("{$v["dta"]["src"]}/{$v["dta"]["nme"]}.png")){
					return "{$v["dta"]["nme"]}.png";
				} else {return false;}
			case "create":
				$img = $this->img(array(
					"act"=>"dwebp",
					"dta"=>array(
						"src"=>$v["dta"]["src"],
						"nme"=>$v["dta"]["nme"],
						"ext"=>$v["dta"]["ext"]
				)));
				if(!$img){die();}
				$dims=getimagesize($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
				$ic=$this->ImgC->name($this->ImgC->Var->Std->o["bc"][2]);
				$ic['x']=$ic['x']*2;
				$ic['y']=$ic['y']*2;
				if($ic['x']!=0 || $ic['y']!=0){
					if($ic['x']==0){
						$ratio = array('x'=>(1/$dims[1])*$dims[0]);
						$scale =array('x'=>$ic['y']*$ratio['x']);
						$size=array('x'=>ceil($scale['x']),'y'=>$ic['y']);
					} elseif($ic['y']==0){
						$ratio = array('y'=>(1/$dims[0])*$dims[1]);
						$scale =array('y'=>$ic['x']*$ratio['y']);
						$size=array('x'=>$ic['x'],'y'=>ceil($scale['y']));
					} else {
						$ratio = array('x'=>(1/$dims[1])*$dims[0],'y'=>(1/$dims[0])*$dims[1]);
						$scale =array('x'=>$ic['y']*$ratio['x'],'y'=>$ic['x']*$ratio['y']);
						$resize = array('x'=>$ic['x']-$scale['x'],'y'=>$ic['y']-$scale['y']);
						if($resize['x']<0 || $resize['y']<0){
							$resize['x']<0 ? $resize='y':$resize='x';
						} else {$resize['x']>$resize['y'] ? $resize='y':$resize='x';}
						if($resize=='y'){
							$size=array('x'=>ceil($scale['x']),'y'=>$ic['y']);
						} else {$size=array('x'=>$ic['x'],'y'=>ceil($scale['y']));}
					}
					$timg = imagecreatefrompng($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
					$timg = imagescale($timg, $size['x'], $size['y']);
					$img = imagecreatetruecolor($ic['x'], $ic['y']);
					imagecopy($img, $timg, 0, 0, ($size['x']-$ic['x'])/2, ($size['y']-$ic['y'])/2, $ic['x'], $ic['y']);
				} else {
					$img = imagecreatefrompng($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
				}
				switch($ic['format']){
					case "jpg":
						imagejpeg($img, $v["dta"]["src"]."/".$this->ImgC->Var->Std->o["bc"][2]."/".$v["dta"]["nme"].".jpg", 80);
						unlink($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
						if(isset($timg)){imagedestroy($timg);}
						imagedestroy($img);
						if(file_exists($v["dta"]["src"]."/".$this->ImgC->Var->Std->o["bc"][2]."/".$v["dta"]["nme"].".jpg")){
							header("Location: /".$this->ImgC->Var->Std->o["uri"]);
						}
						break;
					case "png":
						imagepng($img, $v["dta"]["src"]."/".$this->ImgC->Var->Std->o["bc"][2]."/".$v["dta"]["nme"].".png", 9, PNG_ALL_FILTERS);
						unlink($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
						if(isset($timg)){imagedestroy($timg);}
						imagedestroy($img);
						if(file_exists($v["dta"]["src"]."/".$this->ImgC->Var->Std->o["bc"][2]."/".$v["dta"]["nme"].".png")){
							header("Location: /".$this->ImgC->Var->Std->o["uri"]);
						}
						break;
				}
				die();
		}
	}
}