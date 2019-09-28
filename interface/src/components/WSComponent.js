import React from 'react';
import Sockette from 'sockette';


export const wsComponent = (wsUrl, FormComponent) => class WSComponent extends React.Component {

  componentDidMount() {
    this.onOpen = this.onOpen.bind(this);
    this.onClose = this.onClose.bind(this);
    this.onMessage = this.onMessage.bind(this);
    this.saveData = this.saveData.bind(this);
    this.setDataAndReset = this.setDataAndReset.bind(this);
    const ws = new Sockette(wsUrl, {
      onmessage: this.onMessage,
      onopen: this.onOpen,
      onclose: this.onClose,
      // todo.. other events... change state to reflect connectivity
    });
    this.setState({
      ws,
      connected: false,
      clientId: null,
      data: null,
      fetched: false,
      errorMessage: null
    });
  }

  componentWillUnmount() {
    this.state.ws.close();
  }

  onOpen(event) {
    this.setState({ connected: true });
  }

  onClose(event) {
    this.setState({ connected: false });
  }

  onMessage(event) {
    const data = event.data;
    if (typeof data === 'string' || data instanceof String) {
      var jsonData = JSON.parse(data);

      // message will be either a client id, or a payload
      if (jsonData.client_id) {
        this.setState({ clientId: jsonData.client_id });
      } else if (jsonData.payload) {
        const { clientId, fetched } = this.state;
        // ignore updates for payloads which originate from us
        if (!fetched || jsonData.origin_id !== clientId) {
          this.setState(
            { data: jsonData.payload, fetched: true, errorMessage: null }
          );
        }
      }
    }
  }

  setDataAndReset(data) {
    this.setState({
      data: data,
      fetched: false,
      errorMessage: null
    }, this.saveData);
  }

  saveData() {
    const { ws, connected, data } = this.state;
    if (connected) {
      ws.json(data);
    }
  }

  handleChange = (name, callback) => value => {
    const { data } = this.state;
    data[name] = value;
    this.setState({ data }, callback);
  };

  handleColorChange = (name, callback) => color => {
    const { data } = this.state;
    data[name] = color.hex;
    this.setState({ data }, callback);
  };

  handleValueChange = (name, callback) => event => {
    const { data } = this.state;
    data[name] = event.target.value;
    this.setState({ data }, callback);
  };

  handleCheckboxChange = (name, callback) => event => {
    const { data } = this.state;
    data[name] = event.target.checked;
    this.setState({ data }, callback);
  }

  render() {
    return <FormComponent
      saveData={this.saveData}
      setDataAndReset={this.setDataAndReset}
      handleValueChange={this.handleValueChange}
      handleCheckboxChange={this.handleCheckboxChange}
      handleColorChange={this.handleColorChange}
      handleChange={this.handleChange}
      {...this.state}
      {...this.props}
    />;
  }

}


