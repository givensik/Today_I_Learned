import React from 'react'
import {useEffect} from 'react'
// https://velog.io/@rxxdo/%EB%A6%AC%EC%95%A1%ED%8A%B8%EB%A1%9C-%EB%84%A4%EC%9D%B4%EB%B2%84-%EC%86%8C%EC%85%9C-%EB%A1%9C%EA%B7%B8%EC%9D%B8-%EC%A0%81%EC%9A%A9%ED%95%98%EA%B8%B0-1%EB%B6%80-%EB%84%A4%EC%95%84%EB%A1%9C-%EC%A0%81%EC%9A%A9%ED%95%98%EA%B8%B0

function NaverLogin() {
    // 네이버 공식문서
    // function Naver(){
    //     var naver_id_login = new window.naver_id_login("KVS1mzsMhLIa5R3Ab3Dl", "http://localhost:3000/NaverCallback");
    //     var state = naver_id_login.getUniqState();
    //     naver_id_login.setButton("white", 2, 40);
    //     naver_id_login.setDomain("http://localhost:3000");
    //     naver_id_login.setState(state);
    //     naver_id_login.setPopup(false);
    //     naver_id_login.init_naver_id_login();
    //     naver_id_login.popup= false;
    //     console.log(naver_id_login);
    // }
    
	// return (
    //     <div id ="naver_id_login" >
    //         <button onClick={Naver}>NaverLogin</button>
    //     </div>
	// )
    const { naver } = window
	const NAVER_CLIENT_ID = "KVS1mzsMhLIa5R3Ab3Dl" // 발급 받은 Client ID 입력 
	const NAVER_CALLBACK_URL = "http://localhost:3000/NaverCallback" // 작성했던 Callback URL 입력

	const initializeNaverLogin = () => {
		const naverLogin = new naver.LoginWithNaverId({
			clientId: NAVER_CLIENT_ID,
			callbackUrl: NAVER_CALLBACK_URL,
          // 팝업창으로 로그인을 진행할 것인지?           
			isPopup: true,
          // 버튼 타입 ( 색상, 타입, 크기 변경 가능 )
			loginButton: { color: 'green', type: 3, height: 58 },
			callbackHandle: true,
		})
		naverLogin.init()

           // 선언된 naverLogin 을 이용하여 유저 (사용자) 정보를 불러오는데  
           // 함수 내부에서 naverLogin을 선언하였기에 지역변수처리가 되어  
           // userinfo 정보를 추출하는 것은 지역변수와 같은 함수에서 진행주어야한다.
		
           // 아래와 같이 로그인한 유저 ( 사용자 ) 정보를 직접 접근하여 추출가능하다.
           // 이때, 데이터는 첫 연동시 정보 동의한 데이터만 추출 가능하다.
    
           // 백엔드 개발자가 정보를 전달해준다면 아래 요기! 라고 작성된 부분까지는 
           // 코드 생략이 가능하다.  
      
      naverLogin.getLoginStatus(async function (status) {
			if (status) {
              // 아래처럼 선택하여 추출이 가능하고, 
				const userid = naverLogin.user.getEmail()
				const username = naverLogin.user.getName()
              // 정보 전체를 아래처럼 state 에 저장하여 추출하여 사용가능하다. 
              // setUserInfo(naverLogin.user)
			}
		})     
            // 요기!
	}
    
    
    
            // 네이버 소셜 로그인 (네아로) 는 URL 에 엑세스 토큰이 붙어서 전달된다.
            // 우선 아래와 같이 토큰을 추출 할 수 있으며,
            // 3부에 작성 될 Redirect 페이지를 통해 빠르고, 깨끗하게 처리가 가능하다.
   
	    const userAccessToken = () => {
		    window.location.href.includes('access_token') && getToken()
	}
        
      	const getToken = () => {
		const token = window.location.href.split('=')[1].split('&')[0]
             // console.log, alert 창을 통해 토큰이 잘 추출 되는지 확인하자! 
        		
             // 이후 로컬 스토리지 또는 state에 저장하여 사용하자!   
                // localStorage.setItem('access_token', token)
		        // setGetToken(token)
            return token
	}

        
             // 화면 첫 렌더링이후 바로 실행하기 위해 useEffect 를 사용하였다.
	useEffect(() => {
		initializeNaverLogin()
		userAccessToken()
        console.log()
	}, [])


	return (
		<>
			<div id="naverIdLogin"></div>
		</>
	)
}

export default NaverLogin