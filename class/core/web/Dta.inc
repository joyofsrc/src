<?php

class cwDta {
	
	public function __construct($Cfg){
		$this->Cfg=$Cfg;
		$this->o = array(
			"filter"=>"",
			"order"=>"",
			"limit"=>"",
			"calls"=>0,
			"db"=>new mysqli(
				"p:".$this->Cfg->o['hst'],
				$this->Cfg->o['dbUsr'],
				$this->Cfg->o['dbPss'],
				$this->Cfg->o['dbNme'],
				0,
				$this->Cfg->o['dbSck']
			)
		);
	}
	
	public function clear($v){
		$r = in_array("all",$v) ? true : false;
		foreach(array("filter","order","limit") as $rs){
			if(in_array($rs,$v) || $r){$this->o[$rs] ="";}	
		}
	}
	
	public function filter($v){
		$this->o["filter"] = "WHERE (" . $v . ") ";
	}
	
	public function order($v){
		$r=array("","");
		foreach($v as $key=>$val){
			$r[1] .= "{$r[0]}{$key} {$val} ";
			$r[0]=", ";
		}
		$this->o["order"]="ORDER BY ".$r[1];
	}
	
	public function limit($a,$b=false){
		$this->o["limit"] = "LIMIT {$a}";
		if($b){$this->o["limit"] .= ",{$b}";} 
	}
		
	public function db($v){
		$v["act"]=strtolower($v["act"]);
		switch($v["act"]){
			case "st":
			case "show tables":
				return array_column(mysqli_fetch_all($this->o["db"]->query("SHOW TABLES")),0);
			case "s":
			case "select":
				$q = "SELECT " . $v["query"] . " " . $this->o["filter"] . $this->o["order"] . $this->o["limit"];
				if( !$this->o["db"]->real_query($q) ) {
					throw new exception( $this->o["db"]->error, $this->o["db"]->errno );
				}
				$this->o["calls"]++;
				$r = new mysqli_result($this->o["db"]);
				$flds = $r->fetch_fields();
				foreach ($flds as $fld){
					$cols[] = $fld->name;
					$tbls[] = $fld->table;
				}
				for ($a=0; $a<$r->num_rows; $a++) {
					$row = $r->fetch_array();
					for ($b=0; $b<count($cols); $b++){
						$rs[$a]["{$tbls[$b]}.{$cols[$b]}"]=$row[$b];
					}
				}
				if(isset($rs)){return $rs;} else {return false;}
		}
	}
}