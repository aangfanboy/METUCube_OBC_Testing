import pandas as pd
from dash import Dash, dcc, html
from dash.dependencies import Input, Output
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# CSV file path
FILE_PATH = "power_data.csv"

# Initialize the Dash app
app = Dash(__name__)
app.title = "Real-Time Power Data Viewer"

# App layout
app.layout = html.Div([
    html.H1("Real-Time Power Data Monitor"),
    dcc.Graph(id='live-graph', config={"displayModeBar": False}),
    dcc.Interval(
        id='interval-component',
        interval=2*1000,  # Update every 2 seconds
        n_intervals=0
    )
])

@app.callback(
    Output('live-graph', 'figure'),
    Input('interval-component', 'n_intervals')
)
def update_graph(n):
    try:
        # Read the CSV
        data = pd.read_csv(FILE_PATH, parse_dates=["Time"])

        # Create the figure
        fig = make_subplots(specs=[[{"secondary_y": True}]])
        fig.add_trace(
            go.Scatter(x=data["Time"], y=data["Voltage"], mode='lines+markers', name='Voltage (V)', line=dict(color='blue')),
            secondary_y=False
        )
        fig.add_trace(
            go.Scatter(x=data["Time"], y=data["Current"], mode='lines+markers', name='Current (A)', line=dict(color='red')),
            secondary_y=True
        )

        # Layout
        fig.update_layout(
            title='Power Data Over Time',
            xaxis_title='Time',
            legend=dict(x=0, y=1),
            template='plotly_white',
            height=500
        )
        fig.update_xaxes(rangeslider_visible=True)
        fig.update_yaxes(title_text="Voltage (V)", secondary_y=False)
        fig.update_yaxes(title_text="Current (A)", secondary_y=True)

        return fig

    except Exception as e:
        # Return empty figure in case of failure
        return go.Figure()

if __name__ == "__main__":
    app.run(debug=True)
