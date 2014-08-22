<?php

class cwPge {
	
	public function __construct($Std){
		$this->Std=$Std;
		$this->o = array();
	}
	
	public function fetch($v){
		switch($v["act"]){
			case "id":
				if(!array_key_exists("dta",$v)){return false;}
				if(!is_numeric($v["dta"])){return false;}
				if(array_key_exists("nid-".$v["dta"],$this->o)){
					return $this->o["nid-".$v];
				}
				$this->Std->Dta->clear(array(
					"all"
				));
				$this->Std->Dta->limit(1);
				$rs = $this->Std->Dta->db(array(
					"act"=>"s",
					"query"=>"`pge`.* FROM `pge`"
				));
				if($rs){
					foreach($rs[0] as $r["key"]=>$r["val"]){
						if($this->Std->json(array("act"=>"t","dta"=>$r["val"]))){
							$rs[0][$r["key"]] = $this->Std->json(array("act"=>"d","dta"=>$r["val"]));
						}
						$rs[0][str_replace("pge.","",$r["key"])] = $rs[0][$r["key"]];
						unset($rs[0][$r["key"]]);
					}
					$this->o["nid-".$rs[0]["id"]] = $rs[0];
					return $rs[0];
				}
				return false;
			case "bc":
				
				break;
		}
		return array_key_exists($v,$this->o) ? $this->o[$v] : false;
	}
}