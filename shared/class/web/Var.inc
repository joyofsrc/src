<?php

class cwVar {
	
	public function __construct($Std){
		$this->Std=$Std;
		$this->Std->Dta->clear(array(
			"all"
		));
		$this->Std->Dta->order(array(
			"`var`.`name`"=>"ASC"
		));
		$rs = $this->Std->Dta->db(array(
			"act"=>"s",
			"query"=>"`var`.`name`, `var`.`value` FROM `var`"
		));
		if($rs){
			foreach($rs as $r){
				$this->o[$r["var.name"]]=$this->Std->json(array(
					"act"=>"d",
					"dta"=>$r["var.value"]
				));
			}
		}
	}
	
	public function fetch($v){
		return array_key_exists($v,$this->o) ? $this->o[$v] : false;
	}
	
	public function mtf($v){
		$mfiles=0;
		foreach($this->o[$v["typ"]][$v["dta"]] as $r){
			if(substr($r,0,1)=="/"){$r=substr($r,1);}
			$mfile = filemtime($this->Std->o["core"]."/{$r}");
			if($mfile > $mfiles){$mfiles=$mfile;}
		}
		return $mfiles;
	}
	
	public function compress($v){
				
		$rs="";
		
		/* requests for css resources potentially include scss files, so call the scss class */
		if($v["typ"]=="css"){
			$scss = $this->Std->Dta->Cfg->lc("Scss",$this->Std);
		}
		
		/* resolve the latest mtime for passed resources */
		$mfiles=$this->mtf(array("typ"=>$v["typ"],"dta"=>$v["dta"]));
		
		/* loop through the uncompressed files */
		foreach($this->o[$v["typ"]][$v["dta"]] as $r){
			if(substr($r,0,1)=="/"){$r=substr($r,1);}
			$t = file_get_contents($this->Std->o["core"]."/{$r}");
			if(pathinfo($r,PATHINFO_EXTENSION)=="scss"){
				$t=$scss->compile($t);
			}
			$rs.=$t;
		}
			
		/* remove any comments */
		$rs = preg_replace('#/\*.*?\*/#s', '', $rs);
			
		/* remove any carriage returns or whitespace */
		$rs = str_replace(array("\r\n", "\r", "\n", "\t", '  ', '    ', '    '), '', $rs);
		
		/* delete any old files still held */
		$ofiles = $this->Std->directory($this->Std->o[$v["dta"]],$v["typ"]);
		if($ofiles){
			foreach($ofiles as $ofile){
				unlink($this->Std->o[$v["dta"]]."/".$ofile);
			}
		}

		/* save the updated file */
		file_put_contents($this->Std->o[$v["dta"]]."/".$mfiles.".".$v["typ"],$rs);

		switch($v["typ"]){
			case "css":
				$ftype="text/css";
				break;
			case "js":
				$ftype="text/javascript";
				break;
			default:
				$ftype="application/octet-stream";
				break;
		}
		
		ob_start("ob_gzhandler");
			$this->Std->header(array("ma"=>31536000,"ctyp"=>$ftype,"lm"=>$mfiles));		
			echo $rs;
		ob_end_flush();		
	}
}