import React, { useEffect } from 'react'

function NaverCallback() {
    // useEffect(()=>{
    //     var naver_id_login = new window.naver_id_login("KVS1mzsMhLIa5R3Ab3Dl", "http://localhost:3000/NaverCallback");
        
    //     console.log(naver_id_login.oauthParams.access_token);
    //     // 네이버 사용자 프로필 조회
    //     naver_id_login.get_naver_userprofile(naverSignInCallback());
    //     // 네이버 사용자 프로필 조회 이후 프로필 정보를 처리할 callback function
    //     function naverSignInCallback() {
    //         console.log((naver_id_login.getProfileData('email')));
    //         console.log((naver_id_login.getProfileData('nickname')));
    //         console.log((naver_id_login.getProfileData('age')));
    //     }
        
    // }, [])
    const token = window.location.href.split('=')[1].split('&')[0]
    
  return (
    <div>
        hello
    </div>
  )
}

export default NaverCallback