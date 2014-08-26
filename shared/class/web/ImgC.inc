<?php

class cwImgC {
	
	public function __construct($Var){
		$this->Var=$Var;
		$this->Var->Std->Dta->clear(array(
			"all"
		));
		$this->Var->Std->Dta->order(array(
			"`imgc`.`name`"=>"ASC"
		));
		$rs = $this->Var->Std->Dta->db(array(
			"act"=>"s",
			"query"=>"`imgc`.* FROM `imgc`"
		));
		if($rs){
			for($i=0;$i<count($rs);$i++){
				foreach($rs[$i] as $key=>$val){
					if($this->Var->Std->json(array("act"=>"t","dta"=>$val))){
						$this->o[$i][str_replace("imgc.","",$key)]=$this->Var->Std->json(array(
							"act"=>"d",
							"dta"=>$val
						));	
					} else {
						$this->o[$i][str_replace("imgc.","",$key)]=$val;
					}
				}
			}
		}
	}
	
	public function fetch($v){
		if(isset($this->o)){
			foreach($this->o as $r){
				if(strtolower($r["name"])==strtolower($v)){
					return $r;
				}
			}
		}
		return false;
	}
	
	public function name($v){
		if(!is_array($v)){
			$v=explode("x",$v);
		}
		foreach($this->o as $r){
			if($r["x"]==$v[0] && $r["y"]==$v[1]){
				return $r;
			}
		}
		return false;
	}
}