import React from 'react'
import {useEffect} from 'react'
// https://velog.io/@rxxdo/%EB%A6%AC%EC%95%A1%ED%8A%B8%EB%A1%9C-%EB%84%A4%EC%9D%B4%EB%B2%84-%EC%86%8C%EC%85%9C-%EB%A1%9C%EA%B7%B8%EC%9D%B8-%EC%A0%81%EC%9A%A9%ED%95%98%EA%B8%B0-1%EB%B6%80-%EB%84%A4%EC%95%84%EB%A1%9C-%EC%A0%81%EC%9A%A9%ED%95%98%EA%B8%B0

function NaverLogin() {
    function Naver(){
        var naver_id_login = new window.naver_id_login("KVS1mzsMhLIa5R3Ab3Dl", "http://localhost:3000/NaverCallback");
        var state = naver_id_login.getUniqState();
        naver_id_login.setButton("white", 2, 40);
        naver_id_login.setDomain("http://localhost:3000");
        naver_id_login.setState(state);
        naver_id_login.setPopup(false);
        naver_id_login.init_naver_id_login();
        naver_id_login.popup= false;
        console.log(naver_id_login);
    }
    
	return (
        <div id ="naver_id_login" >
            <button onClick={Naver}>NaverLogin</button>
        </div>
	)
}

export default NaverLogin