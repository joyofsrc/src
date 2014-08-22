<?php

class cwHny {
	
	public function __construct($o){
		$this->o=$o;
	}
	
	public function check($ip){
		$host = $this->build_query($ip);
		return $host ? $this->do_query($host) : null;
	}
	
	private function do_query($host){
		$ip = $this->lookup($host);
		return ($ip ? $this->build_response($ip) : null);
	}
	
	private function build_query($ip){
		if (!$this->is_ip($ip)){
			$ip = $this->lookup("$ip");	
		}
		if (!$ip){return null;}
		$ip = $this->flip_ip($ip);
		$ret = "{$this->o["hnyKey"]}.$ip.{$this->o['hnySrv']}";
		return $ret;
	}
	
	private function build_response($ip){
		$ary = explode('.', $ip);
		$ret = array('raw' => $ip,'age' => $ary[1],'threat' => $ary[2]);
		return $ret;
	}
	
	private function lookup($str){
		$ret = gethostbynamel($str);
		if (!$ret || !count($ret) || !$ret[0]){return null;}
		return ($this->is_ip($ret[0]) ? $ret[0] : null);
		}
		
	private function flip_ip($ip){
		$ret = explode(".", $ip);
		$ret = array_reverse($ret);
		$ret = join('.', $ret);
		return $ret;
	}
	
	function is_ip($str){
		return preg_match('/\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/', $str);
	}
}