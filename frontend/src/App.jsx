import { useState, useEffect, useRef } from "react";
import {
  Mic,
  Power,
  Cpu,
  Clock3,
  Settings,
  Send,
  User,
  Bot,
  Activity,
  Terminal,
  Globe,
  Music,
  Calculator,
  FolderOpen,
  Cloud,
} from "lucide-react";

import "./App.css";

const API = "http://localhost:8080";

function App() {
  const [messages, setMessages] = useState([
    {
      type: "bot",
      text: "Hello. I am JARVIS. How can I help you?",
    },
  ]);

  const [input, setInput] = useState("");
  const [listening, setListening] = useState(false);
  const [online, setOnline] = useState(false);
  const [time, setTime] = useState("--:--");
  const recognitionRef = useRef(null);

  // Clock
  useEffect(() => {
    const updateTime = () => {
      setTime(
        new Date().toLocaleTimeString([], {
          hour: "2-digit",
          minute: "2-digit",
        })
      );
    };

    updateTime();

    const timer = setInterval(updateTime, 1000);

    return () => clearInterval(timer);
  }, []);

  // Backend status
  useEffect(() => {
    checkStatus();

    const timer = setInterval(checkStatus, 5000);

    return () => clearInterval(timer);
  }, []);

  const checkStatus = async () => {
    try {
      const response = await fetch(`${API}/api/status`);

      if (response.ok) {
        setOnline(true);
      } else {
        setOnline(false);
      }
    } catch {
      setOnline(false);
    }
  };

  const addMessage = (type, text) => {
    setMessages((prev) => [
      ...prev,
      {
        type,
        text,
      },
    ]);
  };

  const sendCommand = async (command) => {
    const cleanCommand = command.trim();

    if (!cleanCommand) return;

    addMessage("user", cleanCommand);
    setInput("");

    try {
      const response = await fetch(
        `${API}/api/command`,
        {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            command: cleanCommand,
          }),
        }
      );

      const data = await response.json();

      addMessage(
        "bot",
        data.message ||
        "Command processed."
      );
    } catch (error) {
      addMessage(
        "bot",
        "I cannot connect to the JARVIS backend. Start the C++ JARVIS program first."
      );

      setOnline(false);
    }
  };

  const startListening = () => {
    const SpeechRecognition =
      window.SpeechRecognition ||
      window.webkitSpeechRecognition;

    if (!SpeechRecognition) {
      addMessage(
        "bot",
        "Speech recognition is not supported in this browser."
      );

      return;
    }

    if (listening) {
      recognitionRef.current?.stop();
      return;
    }

    const recognition =
      new SpeechRecognition();

    recognition.lang = "en-US";
    recognition.continuous = false;
    recognition.interimResults = false;

    recognition.onstart = () => {
      setListening(true);
    };

    recognition.onresult = (event) => {
      const text =
        event.results[0][0].transcript;

      setInput(text);

      sendCommand(text);
    };

    recognition.onerror = () => {
      setListening(false);
    };

    recognition.onend = () => {
      setListening(false);
    };

    recognitionRef.current = recognition;

    recognition.start();
  };

  const handleSubmit = (event) => {
    event.preventDefault();

    sendCommand(input);
  };

  const quickCommand = (command) => {
    sendCommand(command);
  };

  return (
    <div className="app">
      {/* HEADER */}

      <header className="header">
        <div className="brand">
          <div className="brandIcon">
            <Bot size={28} />
          </div>

          <div>
            <h1>JARVIS</h1>
            <span>AI ASSISTANT</span>
          </div>
        </div>

        <div className="systemStatus">
          <Activity size={20} />

          <span>
            {online
              ? "SYSTEM ONLINE"
              : "SYSTEM OFFLINE"}
          </span>

          <div
            className={`statusDot ${online ? "online" : ""
              }`}
          />
        </div>
      </header>

      {/* MAIN */}

      <main className="main">
        {/* LEFT */}

        <section className="controlPanel">
          <div
            className={`micCircle ${listening ? "listening" : ""
              }`}
            onClick={startListening}
          >
            <div className="micInner">
              <Mic size={62} />
            </div>
          </div>

          <h2>
            {listening
              ? "LISTENING..."
              : "JARVIS READY"}
          </h2>

          <p>
            {listening
              ? "Speak your command"
              : "Your personal AI assistant"}
          </p>

          <button
            className="listenButton"
            onClick={startListening}
          >
            <Mic size={20} />

            {listening
              ? "STOP LISTENING"
              : "START LISTENING"}
          </button>

          {/* STATUS CARDS */}

          <div className="statusGrid">
            <div className="statusCard">
              <Clock3 />
              <div>
                <small>TIME</small>
                <strong>{time}</strong>
              </div>
            </div>

            <div className="statusCard">
              <Cpu />
              <div>
                <small>CPU</small>
                <strong>ONLINE</strong>
              </div>
            </div>

            <div className="statusCard">
              <Power />
              <div>
                <small>SYSTEM</small>
                <strong>
                  {online ? "READY" : "OFFLINE"}
                </strong>
              </div>
            </div>

            <div className="statusCard">
              <Settings />
              <div>
                <small>MODE</small>
                <strong>AI</strong>
              </div>
            </div>
          </div>

          {/* QUICK COMMANDS */}

          <div className="quickCommands">
            <h3>QUICK COMMANDS</h3>

            <div className="quickGrid">
              <button
                onClick={() =>
                  quickCommand(
                    "open browser"
                  )
                }
              >
                <Globe />
                Browser
              </button>

              <button
                onClick={() =>
                  quickCommand(
                    "open calculator"
                  )
                }
              >
                <Calculator />
                Calculator
              </button>

              <button
                onClick={() =>
                  quickCommand(
                    "play music"
                  )
                }
              >
                <Music />
                Music
              </button>

              <button
                onClick={() =>
                  quickCommand(
                    "open files"
                  )
                }
              >
                <FolderOpen />
                Files
              </button>

              <button
                onClick={() =>
                  quickCommand(
                    "weather in Mumbai"
                  )
                }
              >
                <Cloud />
                Weather
              </button>
            </div>
          </div>
        </section>

        {/* RIGHT */}

        <section className="conversation">
          <div className="conversationHeader">
            <div>
              <h2>Conversation</h2>
              <span>
                JARVIS COMMAND CENTER
              </span>
            </div>

            <div
              className={`conversationDot ${online ? "active" : ""
                }`}
            />
          </div>

          <div className="messages">
            {messages.map(
              (message, index) => (
                <div
                  className={`messageRow ${message.type}`}
                  key={index}
                >
                  <div className="messageIcon">
                    {message.type === "bot" ? (
                      <Bot size={18} />
                    ) : (
                      <User size={18} />
                    )}
                  </div>

                  <div className="messageBubble">
                    <span>
                      {message.type ===
                        "bot"
                        ? "JARVIS"
                        : "YOU"}
                    </span>

                    <p>{message.text}</p>
                  </div>
                </div>
              )
            )}
          </div>

          <form
            className="commandBox"
            onSubmit={handleSubmit}
          >
            <input
              value={input}
              onChange={(e) =>
                setInput(e.target.value)
              }
              placeholder="Type a command..."
            />

            <button type="submit">
              <Send size={20} />
            </button>
          </form>
        </section>
      </main>
    </div>
  );
}

export default App;