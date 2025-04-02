# 💬 ft_irc

[![GitHub stars](https://img.shields.io/github/stars/DawnteaStudio/ft_irc?style=social)](https://github.com/DawnteaStudio/ft_irc/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/DawnteaStudio/ft_irc?style=social)](https://github.com/DawnteaStudio/ft_irc/network)
[![GitHub license](https://img.shields.io/github/license/DawnteaStudio/ft_irc)](https://github.com/DawnteaStudio/ft_irc/blob/main/LICENSE)
[![GitHub last commit](https://img.shields.io/github/last-commit/DawnteaStudio/ft_irc)](https://github.com/DawnteaStudio/ft_irc/commits/main)

> “네트워크 프로그래밍의 핵심을 직접 구현하며 실시간 통신의 구조를 배웠습니다.”  
> _- by sewopark && erho

---

## 📌 프로젝트 개요

IRC (Internet Relay Chat) 프로토콜을 기반으로 한 채팅 서버 및 클라이언트를 구현한 프로젝트입니다.  
C++과 소켓 프로그래밍을 활용하여 실시간 다중 사용자 통신 환경을 구축했습니다.

이 프로젝트는 **42서울 본과정의 네트워크 과제** 중 하나로, **RFC1459 명세**를 직접 구현하는 것이 핵심 목표입니다.  
실제 상용 IRC 클라이언트와 통신이 가능해야 하며, 안정성, 확장성, 정확성을 모두 만족시켜야 합니다.

---

## 🛠️ 주요 기능

- socket, poll 등을 활용한 비동기 통신 및 다중 사용자 연결 처리
- 닉네임 등록, 채널 참여, 귓속말 등 IRC 명령어 처리
- 사용자 인증 및 접속 관리
- 클라이언트와의 안정적인 연결 유지 및 에러 처리
- 실제 상용 IRC 클라이언트와의 호환성 검증 (e.g. irssi, weechat, HexChat)

---

## ⚙️ 실행 방법

```bash
$ git clone https://github.com/DawnteaStudio/ft_irc.git
$ cd ft_irc
$ make
$ ./ircserv <port> <password>
```

- 포트: 사용할 포트 번호
- 패스워드: 클라이언트 접속 시 사용할 인증 비밀번호

> 💡 `telnet`, `nc`, 또는 실제 IRC 클라이언트로 테스트 가능

---

## 🧠 트러블슈팅 사례

### 🔹 상용 IRC 클라이언트 미응답 문제

- **원인**: 한 번의 input이 항상 개행으로 끝난다는 잘못된 가정
- **해결**: 반복문으로 개행 문자를 찾을 때마다 명령 처리 → 유연한 버퍼 처리 로직으로 수정

### 🔹 명령어에 대한 응답 누락

- **원인**: IRC 프로토콜 응답 포맷이 조금만 달라도 클라이언트가 무시
- **해결**: 공식 레퍼런스를 분석해 응답 형식을 enum과 매핑하여 엄격히 구현

### 🔹 사용자 상태관리의 복잡성

- **원인**: 채널 입장 여부, 인증 여부, 대기 상태 등을 종합적으로 판단해야 했음
- **해결**: 유저 구조체에 각 상태를 명시적으로 분리하고, 조건 분기를 통해 흐름 제어 간소화

---

## 📁 디렉토리 구조

```
ft_irc/
├── include/
│   ├── Server.hpp
│   ├── Client.hpp
│   └── Channel.hpp
├── src/
│   ├── server/
│   ├── client/
│   └── commands/
├── Makefile
└── README.md
```

---

## 🙌 프로젝트 후기

> IRC 명세를 처음부터 끝까지 구현해보며 소켓 통신과 상태 기반 명령 처리의 구조를 체득했습니다.  
> 실시간 시스템을 설계하는 데 있어 **프로토콜의 엄격함과 일관성**이 얼마나 중요한지를 몸소 체험할 수 있었습니다.

> 특히, **42서울**의 Peer-to-Peer 피드백을 통해 다양한 시각으로 코드를 리뷰받고, 명확하게 설명하며 발표하는 경험 또한 값진 시간이었습니다.

---

## 📜 라이선스

MIT License

---
