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

function App() {
  return (
    <div >
      <Routes>
         <Route path = "/" element ={<Home />} ></Route>
         <Route path ="/naverlogin" element ={<NaverLogin />}></Route>
         <Route path = "/NaverCallback" element ={<NaverCallback />} ></Route>
      </Routes>
    </div>
  );
}

export default App;
