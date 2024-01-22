import { GoogleLogin } from '@react-oauth/google';
import { GoogleOAuthProvider } from '@react-oauth/google';
import { useGoogleLogin } from '@react-oauth/google';

const GoogleLoginButton = () => {

      
  return (
    <GoogleOAuthProvider clientId='595231241361-dc33l37qlm37su518jap6afsbmbugmb5.apps.googleusercontent.com'>
      <GoogleLogin
        onSuccess={(credentialResponse) => {
          console.log(credentialResponse);
        }}
        onError={() => {
          console.log('Login Failed');
        }}
      />
        

    </GoogleOAuthProvider>
  );
};

export default GoogleLoginButton;
