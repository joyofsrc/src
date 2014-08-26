<?php

class cwUsr {
	
	public function __construct($Std){
		$this->Std=$Std;
		session_name("SRC");
		session_start();
		if(isset($_SESSION['id'])){
			$this->Std->Dta->clear(array(
				"all"
			));
			$this->Std->Dta->limit(1);
			$ths->Std->Dta->filter("`usr`.`id`={$_SESSION['id']}");
			$rs = $this->Std->Dta->db(array(
				"act"=>"s",
				"query"=>"`usr`.`id`, `usr`.`dept` FROM `usr`"
			));
			if($rs){
				foreach($rs[0] as $key=>$val){
					if($this->Std->json(array("act"=>"t","dta"=>$val))){
						$this->o[str_replace("usr.","",$key)]=$this->Std->json(array(
							"act"=>"d",
							"dta"=>$val
						));	
					} else {
						$this->o[str_replace("usr.","",$key)]=$val;
					}
				}
			}
		} else {
			$this->o["dept"]=false;
		}
	}
	
	public function visitor(){
		return $this->o["dept"]===false ? true : false;
	}
	
	public function admin(){
		return $this->o["dept"]!==false ? true : false;
	}
}