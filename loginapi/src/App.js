import React from "react";
import {
  BrowserRouter as Router,
  Routes,
  Route,
  Link,
  useRouteMatch,
  useParams,
  BrowserRouter
} from "react-router-dom";

import Home from "./route/Home";
import NaverLogin from "./route/NaverLogin";
import NaverCallback from "./route/NaverCallback";
import GoogleCallback from "./route/GoogleCallback";
import Mypage from "./route/Mypage";

function App() {
  return (
    <div >
      <Routes>
         <Route path = "/" element ={<Home />} ></Route>
         <Route path ="/naverlogin" element ={<NaverLogin />}></Route>
         <Route path = "/NaverCallback" element ={<NaverCallback />} ></Route>
         <Route path = "/GoogleCallback" element ={<GoogleCallback />} ></Route>
         <Route path = "/Mypage" element ={<Mypage />} ></Route>
      </Routes>
    </div>
  );
}

export default App;
