var runUrlScheme = "intent://" + sQuery + "#Intent;scheme=" + param["and_scheme"] + ";package=" + param["and_pkg"] + ";" + market_referrer + "end";
            
chrome25 = uagentLow.search("chrome") > -1 && navigator.appVersion.match(/Chrome\/\d+.\d+/)[0].split("/")[1] > 25;
kitkatWebview = uagentLow.indexOf("naver") != -1 || uagentLow.indexOf("daum") != -1;
            
//console.log(runUrlScheme);
            
// 2. 앱 미설치인 경우
var visitedAt = (new Date()).getTime();
setTimeout( function() {
var nowAt = (new Date()).getTime();
    if ( nowAt - visitedAt < 2000 ) {
                	// 1) 토스연계 -> 웹연동
                	if(parameter.indexOf("TOSS") > -1){
                		if (window.confirm("신한 SOL라이프 앱이 설치되어 있지 않습니다.\n모바일웹 페이지로 이동하시겠습니까?")) {
                  			goMwebExcute(param)
                		}
                	// 2) 그 외 -> intent로 앱스토어 이동 
                  	}else{
                  		if (chrome25 && !kitkatWebview) {
                            var tempWin =  window.open(runUrlScheme);
                            sleep(1000);
                            if(tempWin != null) {
                                tempWin.close();
                            }
                        } else {
                            document.location.href= runUrlScheme;
                        }
                  	}
                }
            }, 1500);