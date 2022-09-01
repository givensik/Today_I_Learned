import React from 'react'
import { Link } from 'react-router-dom'
import NaverLogin from './NaverLogin'
import GoogleLogin from './GoogleLogin'
import GoogleLoginButton from './GoogleLoginButton'
function Home() {
  return (
    <div>
        <NaverLogin/>
        <GoogleLoginButton></GoogleLoginButton>
    </div>
  )
}

export default Home