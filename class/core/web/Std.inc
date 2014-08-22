<?php

class cwStd {
	
	public function __construct($Dta){
		$v["uri"]=$this->mbParseUrl(
				filter_input(INPUT_SERVER,"REQUEST_URI",FILTER_SANITIZE_STRING,FILTER_FLAG_STRIP_LOW)
		);
		if($v["uri"]){
			if (substr($v["uri"]["path"],0,1) == "/"){
				$v["uri"]["path"] = substr($v["uri"]["path"],1);
			}
		} else {
			die();
		}
		$v["root"]=filter_input(INPUT_SERVER,"DOCUMENT_ROOT",FILTER_SANITIZE_STRING);
		$v["core"]=str_replace("/public","",$v["root"]);
		$v["breadcrumb"]=explode("/",$v["uri"]["path"]);
		$this->o=array(
			"npf"=>"/nginx_protected_files",
			"uri"=>$v["uri"]["path"],
			"bc"=>$v["breadcrumb"],
			"cbc"=>end($v["breadcrumb"]),
			"root"=>$v["root"],
			"core"=>$v["core"],
			"tmp"=>$v["core"] . "/tmp",
			"log"=>$v["core"] . "/log",
			"roll"=>$v["core"] . "/rollback",
			"rec"=>$v["core"] . "/recovery",
			"sess"=>$v["core"] . "/session",
			"lay"=>$v["core"] . "/layouts",
			"bl"=>$v["core"] . "/blocks",
			"res"=>$v["core"] . "/resources",
			"cwv"=>$v["core"]. "/resources/compress/web/visitor",
			"hp"=>$v["core"] . "/resources/honeypot",
			"fc"=>$v["core"] . "/resources/files/cache",
			"fic"=>$v["core"]."/resources/files/images/cms",
			"fiwv"=>$v["core"]."/resources/files/images/web/visitor",
			"fiwa"=>$v["core"]."/resources/files/images/web/admin",
			"fiwm"=>$v["core"]."/resources/files/images/web/member",
			"fdwv"=>$v["core"]."/resources/files/documents/web/visitor",
			"fdwa"=>$v["core"]."/resources/files/documents/web/admin",
			"fdwm"=>$v["core"]."/resources/files/documents/web/member",
			"rem"=>filter_input(INPUT_SERVER, "REMOTE_ADDR",FILTER_VALIDATE_IP),
			"dom"=>filter_input(INPUT_SERVER, "HTTP_HOST",FILTER_SANITIZE_STRING, FILTER_FLAG_STRIP_LOW),
			"tme"=>time(),
			"mtme"=>microtime(true),
			"com"=> filter_input(INPUT_SERVER,"HTTPS",FILTER_SANITIZE_STRING,FILTER_FLAG_STRIP_LOW)=="off" ? "http" : "https",
		);
		$this->Dta=$Dta;
	}
	
	public function mbParseUrl($v){
		$enc_url = preg_replace_callback(
				'%[^:/@?&=#]+%usD',
				function ($matches){
					return urlencode($matches[0]);
				},
				$v
		);
		$parts = parse_url($enc_url);
		if($parts === false){return false;}
		foreach($parts as $name => $value){
			$parts[$name] = urldecode($value);
		}
		return $parts;
	}
	
	public function json($v){
		$v["act"]=mb_strtolower($v["act"]);
		switch($v["act"]){
			case "t":
			case "test":
				$r=array(
					mb_substr($v["dta"],0,1),
					mb_substr($v["dta"],-1)
				);
				return ($r[0]=="{" && $r[1]=="}") || ($r[0]=="[" && $r[1]=="]") ? true : false;
			case "d":
			case "de":
			case "decode":
				$r =json_decode($v["dta"],true,10);
				return $r!=NULL ? $r : false;
			default:
				$v["dta"] = json_encode($v["dta"],JSON_UNESCAPED_UNICODE | JSON_UNESCAPED_SLASHES);
				return $v["dta"];
		}
	}
	
	public function cypher($v){
		if($v["dta"]!=""){
			$v["act"]=mb_strtolower($v["act"]);
			switch ($v["act"]){
				case "e":
				case "enc":
				case "encrypt":
					$v["dta"] = serialize($v["dta"]);
					$iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_CBC), MCRYPT_DEV_URANDOM);
					$key = pack('H*', $this->Dta->Cfg->o["dbKey"]);
					$mac = hash_hmac('sha256', $v["dta"], substr(bin2hex($key), -32));
					$passcrypt = mcrypt_encrypt(MCRYPT_RIJNDAEL_256, $key, $v["dta"] . $mac, MCRYPT_MODE_CBC, $iv);
					$v["dta"] = base64_encode($passcrypt) . '|' . base64_encode($iv);
					return $v["dta"];
				default:			
					$v["dta"] = explode('|', $v["dta"]);
					$decoded = base64_decode($v["dta"][0]);
					$iv = base64_decode($v["dta"][1]);
					if(strlen($iv) !== mcrypt_get_iv_size(MCRYPT_RIJNDAEL_256, MCRYPT_MODE_CBC)){return false;}
					$key = pack('H*', $this->Dta->Cfg->o["dbKey"]);
					$decrypted = trim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $key, $decoded, MCRYPT_MODE_CBC, $iv));
					$mac = substr($decrypted, -64);
					$decrypted = substr($decrypted, 0, -64);
					$calcmac = hash_hmac('sha256', $decrypted, substr(bin2hex($key), -32));
					if($calcmac !== $mac){return false;}
					$v["dta"] = unserialize($decrypted);
					return $v["dta"];
			}
		}
	}
	
	public function alias($v){
		$a = array(	'á','a',	'Á','A',	'à','a',	'À','A',	'ă','a',	'Ă','A',	'â','a',	'Â','A',	'å','a',	'Å','A',	'ã','a',	'Ã','A',
				'ą','a',	'Ą','A',	'ā','a',	'Ā','A',	'ä','ae',	'Ä','AE',	'æ','ae',	'Æ','AE',	'ḃ','b',	'Ḃ','B',	'ć','c',		'Ć','C',
				'ĉ','c',		'Ĉ','C',	'č','c',		'Č','C',	'ċ','c',		'Ċ','C',	'ç','c',		'Ç','C',	'ď','d',	'Ď','D',	'ḋ','d',	'Ḋ','D',
				'đ','d',	'Đ','D',	'ð','dh',	'Ð','Dh',	'é','e',	'É','E',	'è','e',	'È','E',	'ĕ','e',	'Ĕ','E',	'ê','e',	'Ê','E',
				'ě','e',	'Ě','E',	'ë','e',	'Ë','E',	'ė','e',	'Ė','E',	'ę','e',	'Ę','E',	'ē','e',	'Ē','E',	'ḟ','f',		'Ḟ','F',
				'ƒ','f',		'Ƒ','F',	'ğ','g',	'Ğ','G',	'ĝ','g',	'Ĝ','G',	'ġ','g',	'Ġ','G',	'ģ','g',	'Ģ','G',	'ĥ','h',	'Ĥ','H',
				'ħ','h',	'Ħ','H',	'í','i',		'Í','I',		'ì','i',		'Ì','I',		'î','i',		'Î','I',		'ï','i',		'Ï','I',		'ĩ','i',		'Ĩ','I',
				'į','i',		'Į','I',		'ī','i',		'Ī','I',		'ĵ','j',		'Ĵ','J',		'ķ','k',	'Ķ','K',	'ĺ','l',		'Ĺ','L',		'ľ','l',		'Ľ','L',
				'ļ','l',		'Ļ','L',		'ł','l',		'Ł','L',	'ṁ','m',	'Ṁ','M',	'ń','n',	'Ń','N',	'ň','n',	'Ň','N',	'ñ','n',	'Ñ','N',
				'ņ','n',	'Ņ','N',	'ó','o',	'Ó','O',	'ò','o',	'Ò','O',	'ô','o',	'Ô','O',	'ő','o',	'Ő','O',	'õ','o',	'Õ','O',
				'ø','oe',	'Ø','OE',	'ō','o',	'Ō','O',	'ơ','o',	'Ơ','O',	'ö','oe',	'Ö','OE',	'ṗ','p',	'Ṗ','P',	'ŕ','r',		'Ŕ','R',
				'ř','r',		'Ř','R',	'ŗ','r',		'Ŗ','R',	'ś','s',		'Ś','S',	'ŝ','s',		'Ŝ','S',	'š','s',		'Š','S',	'ṡ','s',		'Ṡ','S',
				'ş','s',		'Ş','S',	'ș','s',		'Ș','S',	'ß','SS',	'ť','t',		'Ť','T',	'ṫ','t',		'Ṫ','T',	'ţ','t',		'Ţ','T',	'ț','t',
				'Ț','T',	'ŧ','t',		'Ŧ','T',	'ú','u',	'Ú','U',	'ù','u',	'Ù','U',	'ŭ','u',	'Ŭ','U',	'û','u',	'Û','U',	'ů','u',
				'Ů','U',	'ű','u',	'Ű','U',	'ũ','u',	'Ũ','U',	'ų','u',	'Ų','U',	'ū','u',	'Ū','U',	'ư','u',	'Ư','U',	'ü','ue',
				'Ü','UE',	'ẃ','w',	'Ẃ','W',	'ẁ','w',	'Ẁ','W',	'ŵ','w',	'Ŵ','W',	'ẅ','w',	'Ẅ','W',	'ý','y',		'Ý','Y',	'ỳ','y',
				'Ỳ','Y',	'ŷ','y',		'Ŷ','Y'		,'ÿ','y',	'Ÿ','Y',	'ź','z',	'Ź','Z',	'ž','z',	'Ž','Z',	'ż','z',	'Ż','Z',	'þ','th',
				'Þ','Th',	'µ','u',	'а','a',	'А','a',	'б','b',	'Б','b',	'в','v',	'В','v',	'г','g',	'Г','g',	'д','d',	'Д','d',
				'е','e',	'Е','e',	'ё','e',	'Ё','e',	'ж','zh',	'Ж','zh',	'з','z',	'З','z',	'и','i',	'И','i',	'й','j',	'Й','j',
				'к','k',	'К','k',	'л','l',	'Л','l',	'м','m',	'М','m',	'н','n',	'Н','n',	'о','o',	'О','o',	'п','p',	'П','p',
				'р','r',	'Р','r',	'с','s',	'С','s',	'т','t',	'Т','t',	'у','u',	'У','u',	'ф','f',	'Ф','f',	'х','h',	'Х','h',
				'ц','c',	'Ц','c',	'ч','ch',	'Ч','ch',	'ш','sh',	'Ш','sh',	'щ','sch',	'Щ','sch',	'ъ','',		'Ъ','',		'ы','y',	'Ы','y',
				'ь','',		'Ь','',		'э','e',	'Э','e',	'ю','ju',	'Ю','ju',	'я','ja',	'Я','ja'	);
		for($r=0;$r<count($a);$r=$r+2){$v = str_replace($a[$r],$a[$r+1],$v);}
		return mb_strtolower(preg_replace(array('/[^-a-zA-Z0-9_ -]/','/\s+/'),array('','-','-'),$v));
	}
	
	private function formatText($v){
		switch($v["act"]){
			case 1:
				/* prior to storing plain textarea to database */
				return trim($this->Dta->o["db"]->real_escape_string(str_replace(array("\r\n","\n","\r","\t"),array("[format:newline]","[format:newline]","[format:newline]","[format:tab]"),$v["dta"])));	
			case 2:
				/* outputing direct from plain text area to html */
				return str_replace(array("\r\n","\n","\r","\t","[format:newline]","[format:tab]"),array("<br>","<br>","<br>","","<br>",""),$v["dta"]);
			default:
				/* strip carriage returns and tabs */
				return trim($this->Dta->o["db"]->real_escape_string(str_replace(array("\r\n","\n","\r","\t"), "", $v["dta"])));
		}
	}
	
	public function text($a,$tags = 0){
		if(is_array($a)){
			foreach($a as $key => $val){
				if(is_array($val)){
					$a[$key] = $this->text($a[$key],$tags);
				} else {
					$a[$key] = $this->formatText(array("act"=>$tags,"dta"=>$val));
				}
			}
			return $a;
		} else {
			$a = $this->formatText(array("act"=>$tags,"dta"=>$a));
			return $a;
		}
	}
	
	public function debug($v){
		switch($v["act"]){
			case "array":
				$r = '<ul class="pad10">';
				if (is_array($v["dta"])){
					foreach ($v["dta"] as $key=>$val){
						if (is_array($val)){
							$r .= "<li class='pad10'>" . $key . " => " . $this->debug(array("act"=>"array","dta"=>$val)) . "</li>\n";
						}else{
							$r .= "<li class='pad10'>" . $key . " => " . $val . "</li>\n";
						}
					}
				}
			$r .= '</ul>';
			return $r;	
		}
	}
	
	public function header($v){
			
		/* page */
		if(isset($v["ok"])){
			header('HTTP/1.1 200 OK');
			header("Host: ".$this->o["dom"]);
			header("Content-Type: text/html; charset=UTF-8");
			header("Connection: Keep-Alive",true);
			header("Keep-Alive: timeout=2, max=100",true);
			return;
		}
		
		/* redirect */
		if(isset($v["loc"])){
			header('HTTP/1.1 200 OK');
			header("Host: ".$this->o["dom"]);
			header("Content-Type: text/html; charset=UTF-8");
			header("Connection: Keep-Alive",true);
			header("Keep-Alive: timeout=2, max=100",true);
			if(stristr($v["loc"],"http://") || stristr($v["loc"],"https://")){
				header("Location: ".$v["loc"]);
			} else {
				header("Location: ".$this->o["com"]."://".$this->o["dom"].$v["loc"]);
			}
			die();
		}
		
		/* resource */
		$etag = "W/\"".md5($this->o["uri"].$v["lm"])."\"";
		$v["http_if_none_match"] = filter_input(INPUT_SERVER,"HTTP_IF_NONE_MATCH",FILTER_SANITIZE_STRING);
		$v["http_if_modified_since"] = filter_input(INPUT_SERVER,"HTTP_IF_MODIFIED_SINCE",FILTER_SANITIZE_STRING);
		
		if($v["http_if_none_match"]==$etag || $v["http_if_modified_since"]==gmdate('D, d M Y H:i:s', $v["lm"]).' GMT'){
			header('HTTP/1.1 304 Not Modified');
			header("Host: ".$this->o["dom"]);
			header("Content-Type: {$v["ctyp"]}");
			header("Cache-Control: public");
			header('Last-Modified: '.gmdate('D, d M Y H:i:s', $v["lm"]).' GMT');
			header("ETag: {$etag}");
			header("Pragma: cache");
			header("Connection: Keep-Alive");
			header("Keep-Alive: timeout=2, max=100");
			header("Expires: ".gmdate('D, d M Y H:i:s', $this->o["tme"] + $v["ma"])." GMT");
			return 0;
		} else {
			header('HTTP/1.1 200 OK');
			header("Host: ".$this->o["dom"]);
			/*header("Vary: Accept-Encoding");*/
			header("Content-Type: {$v["ctyp"]}");
			header("Content-Encoding: gzip");
			header('Last-Modified: '.gmdate('D, d M Y H:i:s', $v["lm"]).' GMT');
			header("ETag: {$etag}");
			header("Cache-Control: public");
			header("Pragma: cache");
			header("Connection: Keep-Alive");
			header("Keep-Alive: timeout=2, max=100");
			if(isset($v["ma"])){
				header("Cache-Control: max-age={$v["ma"]}");
				header("Expires: ".gmdate('D, d M Y H:i:s', $this->o["tme"] + $v["ma"])." GMT");
			}
			return 1;
		}
	}
	
	public function serve($v){
		switch($v["act"]){
			case "resource":
				$finfo = new finfo(FILEINFO_MIME_TYPE);
				$r = array(
					"lm"=>filemtime($v["dta"]),
					"ma"=>31536000,
					"ctyp"=>array_key_exists("ctyp",$v) ? $v["ctyp"] : $finfo->file($v["dta"])
				);
				ob_start("ob_gzhandler");
				$r = $this->header($r);
				if($r==1){
					$r =  str_replace($this->o["res"],$this->o["npf"],$v["dta"]);
					header('X-Accel-Redirect: '.$r);
				}
				ob_end_flush();
				die();
		}
	}
	
	public function directory($dir,$mime=false){
		if(is_dir($dir)){
			$r=array();
			foreach (new DirectoryIterator($dir) as $fileInfo) {
				if($fileInfo->isDot()){continue;}
				if($mime){
					if($fileInfo->getExtension()==$mime){
						$r[]=$fileInfo->getFilename();
					}
				} else {$r[]=$fileInfo->getFilename();}
			}
			return $r;
		} else { return false;}
	}
}