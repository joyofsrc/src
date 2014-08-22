<?php

class cwCfg {
		
	public function __construct($v){
		$r=file_get_contents($v);
		$this->o=json_decode(
				trim($r),
				true
		);
		if($this->o["dbKey"]==""){
			$iv = mcrypt_create_iv(
					mcrypt_get_iv_size(
							MCRYPT_RIJNDAEL_256,
							MCRYPT_MODE_CBC
					),
					MCRYPT_DEV_URANDOM
			);
			$this->o["dbKey"] = bin2hex($iv);
			$r=json_encode(
					$this->o,
					JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES
			);
			file_put_contents($v,$r);
		}
		$this->o["f"]=$v;
	}
		
	public function lc($class,$dependency){
		$class= in_array($class,$this->o["cor"]) ? "cw{$class}" : "lw{$class}";
		return new $class($dependency);
	}
}