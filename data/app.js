const WS_URL = `ws://${location.hostname}:81`
let ws

let
  initBlock,
  uidValue,
  ssidMainInput,
  passwordMainInput,
  ssidFallbackInput,
  passwordFallbackInput,
  submitBtn,
  processBlock,
  statusBlock,
  resetBtn
  
try {
  ws = new WebSocket(WS_URL, ['arduino'])
} catch (e) {}

const socketHandler = () => {
  if (!ws) {
    return
  }

  ws.onopen = () => {
    // onopen
  }

  ws.onmessage = (e) => {
    let parsedData = {}

    try {
      parsedData = JSON.parse(e.data)
    } catch (e) {}
    
    messagesHandler(parsedData)
  }

  ws.onerror = (e) => {
    // onerror ${e.data}
  }

  ws.onclose = () => {
    // onclose
  }
}

const messagesHandler = ({command, data}) => {
  switch (command) {
    case 'INIT':
      initBlock.style.display = 'block'
      processBlock.style.display = 'none'
      uidValue.innerHTML = data
      break;
    case 'CONNECT':
      initBlock.style.display = 'none'
      processBlock.style.display = 'block'
      break;
  }
}

const sendMessage = (msg) => {
  if (!ws) {
    return
  }

  ws.send(JSON.stringify(msg));
}

document.addEventListener('DOMContentLoaded', () => {
  initBlock = document.querySelector('#init-block')
  uidValue = initBlock.querySelector('h2.uid')
  ssidMainInput = initBlock.querySelector('input[name="ssid-main"]')
  passwordMainInput = initBlock.querySelector('input[name="password-main"]')
  ssidFallbackInput = initBlock.querySelector('input[name="ssid-fallback"]')
  passwordFallbackInput = initBlock.querySelector('input[name="password-fallback"]')
  submitBtn = initBlock.querySelector('input[type="submit"]')
  processBlock = document.querySelector('#process-block')
  statusBlock = processBlock.querySelector('.status')
  resetBtn = processBlock.querySelector('input[type="reset"]')

  submitBtn.addEventListener('click', () => {
    initBlock.style.display = 'none'
    processBlock.style.display = 'block'

    const data = {
      ssidMain: ssidMainInput.value,
      passwordMain: passwordMainInput.value,
      ssidFallback: ssidFallbackInput.value,
      passwordFallback: passwordFallbackInput.value,
    }

    sendMessage({
      command: 'CONNECT',
      data
    })
  })

  resetBtn.addEventListener('click', () => {
    initBlock.style.display = 'block'
    processBlock.style.display = 'none'

    sendMessage({
      command: 'RESET'
    })
  })

  socketHandler()
})