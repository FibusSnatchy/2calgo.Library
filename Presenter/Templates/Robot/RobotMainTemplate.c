#References_PLACE_HOLDER##Usings_PLACE_HOLDER#

namespace cAlgo.Robots
{
    [Robot()]
    public class #AlgoName_PLACE_HOLDER# : Robot
    {
#Mq4Functions_PLACE_HOLDER#

[Conditional("SendMail")]
//{
		[Parameter("Email from", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressFrom { get; set; }

        [Parameter("Email to", DefaultValue = "your_email@gmail.com")]
        public string EmailAddressTo { get; set; }
//}

#Parameters_PLACE_HOLDER#

#Mq4Fields_PLACE_HOLDER#
        
	protected override void OnStart()
    {
		CommonInitialize();			            
		#DebugActions_PLACE_HOLDER#           

		Mq4Init();

		var mq4Thread = new Thread(Mq4ThreadStart);
		mq4Thread.Start();
    }

    AutoResetEvent _mq4Start = new AutoResetEvent(false);
    AutoResetEvent _mq4Finished = new AutoResetEvent(false);
    DesiredTrade _desiredTrade;
    Position _positionToProtect;
    Position _lastOpenedPosition;
    PendingOrder _lastPlacedOrder;

    protected override void OnTick()
    {
        if (_desiredTrade != null && _desiredTrade.IsPosition 
            && _positionToProtect != null 
            && (!_desiredTrade.StopLoss.HasValue || _positionToProtect.StopLoss.HasValue) 
            && (!_desiredTrade.TakeProfit.HasValue || _positionToProtect.TakeProfit.HasValue) )
        {
            _positionToProtect = null;
            _desiredTrade = null;
            ExecuteMq4Code();
        }
        else if (_desiredTrade == null)
            ExecuteMq4Code();
    }

    private void ExecuteMq4Code()
    {
        _mq4Start.Set();
        _mq4Finished.WaitOne();             
    }

    protected override void OnPositionOpened(Position openedPosition)
    {
        _lastOpenedPosition = openedPosition;
        if (_desiredTrade != null
            && _desiredTrade.IsPosition == true)
        {
            if (_desiredTrade.StopLoss == null && _desiredTrade.TakeProfit == null)
            {
                _desiredTrade = null;
                ExecuteMq4Code();
            }
            else
            {
                _positionToProtect = openedPosition;
                Trade.ModifyPosition(openedPosition, _desiredTrade.StopLoss, _desiredTrade.TakeProfit);
            }
        }
    }

    protected override void OnPendingOrderCreated(PendingOrder newOrder)
    {
        _lastPlacedOrder = newOrder;
        _desiredTrade = null;
        ExecuteMq4Code();
    }

    private void Mq4ThreadStart()
    {
    	while (_mq4Start.WaitOne())
    	{
			try
			{
				Mq4Start();
			}
			catch(Exception e)
			{
				#HandleException_PLACE_HOLDER#
			}
    		_mq4Finished.Set();
    	}
    }

	private IndicatorDataSeries CreateDataSeries()
	{
		return null;
	}

    private bool IsLastBar
    {
        get { return true; }
    }

#InnerParts_PLACE_HOLDER#
#RobotInnerParts_PLACE_HOLDER#

	}

	//Custom Indicators Place Holder

#OuterParts_PLACE_HOLDER#

}