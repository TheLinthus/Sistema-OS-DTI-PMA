<?php 

class Login {
    private static function decodefromcookie($str) {
        $ar = array();
        $exp = explode('*', $str);
        foreach ($exp as $v) {
            list($key, $value) = explode('*', base64_decode($v));
            if (strpos($value, '(array)') !== false) {
                $ar[base64_decode($key)] = self::decodefromcookie(base64_decode(str_replace('(array)', '', $value)));
            } else {
                $ar[base64_decode($key)] = base64_decode($value);
            }
        }
        return $ar;
    }
    
    private static function encodeforcookie($ar) {
        $str = '';
        foreach ($ar as $key => $value) {
            if (is_array($value)) {
                $str .= base64_encode(base64_encode($key) . '*(array)' . base64_encode(self::encodeforcookie($value))) . '*';
            } else {
                $str .= base64_encode(base64_encode($key) . '*' . base64_encode($value)) . '*';
            }
        }
        return rtrim($str, '*');
    }
    
    private static function updateCookie($rs) {
        setcookie('sos_login', $rs, time() + 3600 * 24 * 7, '/');
    }

    static function loginUsuario(&$rs) {
        $rs['nivel'] = 0;
        self::updateCookie(self::encodeforcookie($rs));
        foreach ($rs as $key => $value) {
            $_SESSION[$key] = $value;
        }

        session_commit();
    }

    static function loginTecnico(&$rs) {
        if (!isset($rs['nivel']) || $rs['nivel'] < 1) {
            $rs['nivel'] = 2;
        }

        self::updateCookie(self::encodeforcookie($rs));
        foreach ($rs as $key => $value) {
            $_SESSION[$key] = $value;
        }

        session_commit();
    }

    static function logout() {
        if (isset($_COOKIE['sos_login'])) {
            unset($_COOKIE['sos_login']);
            setcookie('sos_login', null, time() - 3600, '/');
        }

        session_destroy();
        session_unset();
    }

    static function isLogedIn() {
        if (isset($_SESSION['nivel'])) {
            if (isset($_COOKIE['sos_login'])) {
                self::updateCookie($_COOKIE['sos_login']);
            }
            return true;
        } else {
            if (isset($_COOKIE['sos_login'])) {
                $rs = self::decodefromcookie($_COOKIE['sos_login']);
                foreach ($rs as $key => $value) {
                    $_SESSION[$key] = $value;
                }
                return true;
            }
            return false;
        }
    }

    static function isDti() {
        return $_SESSION['nivel'] >= 1;
    }

    static function isTriagem() {
        return $_SESSION['triagem'] == 1 || $_SESSION['nivel'] == 1 || $_SESSION['nivel'] == 3;
    }

    static function isTecnico() {
        return $_SESSION['nivel'] == 2 || $_SESSION['nivel'] == 3;
    }

    static function isAdmin() {
        return $_SESSION['nivel'] == 3;
    }
}