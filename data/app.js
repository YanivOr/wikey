const WS_URL = `ws://${location.hostname}:81`
let ws

let
  initBlock,
  uidValue,
  scanBtn,
  ssidMainSelect,
  passwordMainInput,
  ssidFallbackSelect,
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
    sendMessage({
      command: 'SCAN'
    })
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
    location.reload()
  }
}

const encryptionToString = (encryption) => {
  const encryptions = {
    2: 'TKIP (WPA)',
    5: 'WEP',
    4: 'CCMP (WPA)',
    7: 'NONE',
    8: 'AUTO',
  };

  return encryptions[encryption];
}

const messagesHandler = ({command, data}) => {
  switch (command) {
    case 'SCAN':
      let options = `<option value=""></option>`
      data.map(({ssid, rssi, encryption}) => {
        options += `<option value="${ssid}">${ssid} - ${rssi}dBm - ${encryptionToString(encryption)}</option>`
      })
      ssidMainSelect.innerHTML = options
      ssidFallbackSelect.innerHTML = options
      break;
    case 'INIT':
      initBlock.style.display = 'block'
      processBlock.style.display = 'none'
      uidValue.innerHTML = data
      break;
    case 'CONNECT':
      initBlock.style.display = 'none'
      processBlock.style.display = 'block'

      const {ssid, status} = data

      if (ssid && status) {
        statusBlock.classList.add(status)
        statusBlock.innerHTML = `
          <div><strong>SSID</strong>: ${ssid}</div>
          <div><strong>Status</strong>: ${status}</div>
        `
      }
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
  scanBtn = initBlock.querySelector('button.scan')
  ssidMainSelect = initBlock.querySelector('select[name="ssid-main"]')
  passwordMainInput = initBlock.querySelector('input[name="password-main"]')
  ssidFallbackSelect = initBlock.querySelector('select[name="ssid-fallback"]')
  passwordFallbackInput = initBlock.querySelector('input[name="password-fallback"]')
  submitBtn = initBlock.querySelector('input[type="submit"]')
  processBlock = document.querySelector('#process-block')
  statusBlock = processBlock.querySelector('.status-block')
  resetBtn = processBlock.querySelector('input[type="reset"]')

  scanBtn.addEventListener('click', () => {
    sendMessage({
      command: 'SCAN'
    })
  })

  submitBtn.addEventListener('click', () => {
    if (!confirm('Save configurations?')) {
      return
    }

    initBlock.style.display = 'none'
    processBlock.style.display = 'block'

    const data = {
      ssidMain: ssidMainSelect.value,
      passwordMain: passwordMainInput.value,
      ssidFallback: ssidFallbackSelect.value,
      passwordFallback: passwordFallbackInput.value,
    }

    sendMessage({
      command: 'CONNECT',
      data
    })
  })

  resetBtn.addEventListener('click', () => {
    if (!confirm('Reset configurations?')) {
      return
    }

    initBlock.style.display = 'block'
    processBlock.style.display = 'none'

    sendMessage({
      command: 'RESET'
    })
  })

  socketHandler()
})