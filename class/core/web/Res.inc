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
						/* TODO  - scale crop code */
					} elseif($ic['y']==0){
						/* TODO  - scale crop code */
					} else {
						$ratio = array(
							'x'=>(1/$dims[1])*$dims[0],
							'y'=>(1/$dims[0])*$dims[1]
						);
						$scale =array(
							'x'=>$ic['y']*$ratio['x'],
							'y'=>$ic['x']*$ratio['y']
						);
						$resize = array(
							'x'=>$ic['x']-$scale['x'],
							'y'=>$ic['y']-$scale['y']
						);
						if($resize['x']<0 || $resize['y']<0){
							$resize['x']<0 ? $resize='y':$resize='x';
						} else {
							$resize['x']>$resize['y'] ? $resize='y':$resize='x';
						}
						if($resize['x']){
							$size=array(
								'x'=>ceil($scale['x']),
								'y'=>$ic['y']
							);
						} else {
							$size=array(
								'x'=>$ic['x'],
								'y'=>ceil($scale['y'])
							);
						}
						/* TODO - fix black border at bottom of image (scaling is going wrong) */
						$img = imagecreatefrompng($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
						$img = imagescale($img, $size['x'], $size['y'],  IMG_BICUBIC);
						$img = imagecrop($img,array(
							'x'=>($size['x']-$ic['x'])/2,
							'y'=>($size['y']-$ic['y'])/2,
							'width'=>$ic['x'],
							'height'=>$ic['y']
						));
					}
				} else {
					$img = imagecreatefrompng($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
				}
				
				switch($ic['format']){
					case "jpg":
						/* TODO we need to scale / crop / apply effects */
						imagejpeg($img, $v["dta"]["src"]."/".$this->ImgC->Var->Std->o["bc"][2]."/".$v["dta"]["nme"].".jpg", 80);
						unlink($v["dta"]["src"]."/".$v["dta"]["nme"].".png");
						break;
					case "png":
						break;
				}
				imagedestroy($img);
				/* TODO grab details regarding the IC and create the png/jpg, then destroy the gd image and the actual image */
				/* finally serve the actual image */
				//imagejpeg($img, './example.jpeg', 100);
				//imagedestroy($img);
				break;
		}
	}
}