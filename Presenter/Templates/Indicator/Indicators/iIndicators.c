
        private static MovingAverageType ToMaType(int constant)
        {
            switch (constant)
            {
                case MODE_SMA:
                    return MovingAverageType.Simple;
                case MODE_EMA:
                    return MovingAverageType.Exponential;
                case MODE_LWMA:
                    return MovingAverageType.Weighted;
                default:
                  throw new ArgumentOutOfRangeException("Not supported moving average type");
            }
        }

		[Conditional("iMA", "iMAOnArray", "iBearsPower", "iBullsPower")]
		//{
#region iMA
        private double iMA(string symbol, int timeframe, int period, int ma_shift, int ma_method, int applied_price, int shift)
        {
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateiMA(series, period, ma_method, shift);
        }       
                
        private double iMAOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int ma_shift, int ma_method, int shift) 
        {
          return CalculateiMA(invertedDataSeries.OutputDataSeries, period, ma_method, shift);
        } 
                
        private double iMAOnArray(Mq4Array<Mq4Double> mq4Array, int total, int period, int ma_shift, int ma_method, int shift) 
        {
			var dataSeries = _mq4ArrayToDataSeriesConverterFactory.Create(mq4Array);
			return CalculateiMA(dataSeries, period, ma_method, shift);
        }
        
        private double CalculateiMA(DataSeries dataSeries, int period, int ma_method, int shift)
        {     
            if (ma_method == MODE_SMMA)
            {
                return CalculateWellesWilderSmoothing(dataSeries, period, shift);
            }

            var maType = ToMaType(ma_method);            
            var indicator = _cachedStandardIndicators.MovingAverage(dataSeries, period, maType);

            return indicator.Result.FromEnd(shift);
        }        
        
        private double CalculateWellesWilderSmoothing(DataSeries dataSeries, int period, int shift)
        {
            var indicator = _cachedStandardIndicators.WellesWilderSmoothing(dataSeries, period);
            
            return indicator.Result.FromEnd(shift);
        }
#endregion //iMA
		//}

		[Conditional("iRSI", "iRSIOnArray")]
		//{
#region iRSI
        private double iRSI(Mq4String symbol, int timeframe, int period, int applied_price, int shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateRsi(series, period, shift);
        }       
                
        private double iRSIOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int shift) 
        {
          return CalculateRsi(invertedDataSeries.OutputDataSeries, period, shift);
        } 
                
        private double iRSIOnArray(Mq4Array<Mq4Double> mq4Array, int total, int period, int shift) 
        {
			var dataSeries = _mq4ArrayToDataSeriesConverterFactory.Create(mq4Array);
			return CalculateRsi(dataSeries, period, shift);
        }
        
        private double CalculateRsi(DataSeries dataSeries, int period, int shift)
        {     
            var indicator = _cachedStandardIndicators.RelativeStrengthIndex(dataSeries, period);
            return indicator.Result.FromEnd(shift);
        }

#endregion //iRSI    
		//}

		[Conditional("iBands", "iBandsOnArray")]
		//{
#region iBands
        private double iBands(Mq4String symbol, int timeframe, int period, int deviation, int bands_shift, int applied_price, int mode, int shift)
        {
            if (bands_shift != 0)
                throw new NotImplementedException(NotSupportedBandsShift);
            
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateBands(series, period, deviation, mode, shift);
        }       
                
        private double iBandsOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int period, int deviation, int bands_shift, int mode, int shift) 
        {
            if (bands_shift != 0)
                throw new NotImplementedException(NotSupportedBandsShift);
            
            return CalculateBands(invertedDataSeries.OutputDataSeries, period, deviation, mode, shift);
        }
        
        private double CalculateBands(DataSeries dataSeries, int period, int deviation, int mode, int shift)
        {     
            var indicator = _cachedStandardIndicators.BollingerBands(dataSeries, period, deviation, MovingAverageType.Simple);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.Main.FromEnd(shift);
              case MODE_UPPER:
                return indicator.Top.FromEnd(shift);
              case MODE_LOWER:
                return indicator.Bottom.FromEnd(shift);
            }

            return 0;
        }

#endregion //iBands   		
		//}

		[Conditional("iADX")]
		//{
#region iADX
        private Mq4Double iADX(Mq4String symbol, int timeframe, int period, int applied_price, int mode, int shift)
        {
            if (applied_price != PRICE_CLOSE)            
              throw new NotImplementedException(AdxSupportsOnlyClosePrice);            

			var marketSeries = GetSeries(symbol, timeframe);
      
            return CalculateAdx(marketSeries, period, mode, shift);
        }      
        
        private Mq4Double CalculateAdx(MarketSeries marketSeries, int period, int mode, int shift)
        {     
            var indicator = _cachedStandardIndicators.DirectionalMovementSystem(marketSeries, period);            
            switch (mode)
            {
              case MODE_MAIN:
                return indicator.ADX.FromEnd(shift);
              case MODE_PLUSDI:
                return indicator.DIPlus.FromEnd(shift);
              case MODE_MINUSDI:
                return indicator.DIMinus.FromEnd(shift);
            }
            return 0;
        }

#endregion //iADX    
		//}

		[Conditional("iATR")]
		//{
#region iATR
        private double iATR(Mq4String symbol, int timeframe, int period, int shift)
        {
			var series = GetSeries(symbol, timeframe);
            return CalculateATR(series, period, shift);
        }       
                
        private double CalculateATR(MarketSeries series, int period, int shift)
        {     
            var indicator = _cachedStandardIndicators.ATR(series, period);

            return indicator.Result.FromEnd(shift);            
        }

#endregion //iATR   
		//}

[Conditional("iMACD")]
//{
        private double iMACD(Mq4String symbol, int timeframe, int fast_ema_period, int slow_ema_period, int signal_period, int applied_price, int mode, int shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            var indicator = _cachedStandardIndicators.MacdCrossOver(series, fast_ema_period, slow_ema_period, signal_period);

            switch (mode)
            {
              case MODE_MAIN:
                return indicator.MACD.FromEnd(shift);
              default:
                return indicator.Signal.FromEnd(shift);
            }
        }       
//}
[Conditional("iOsMA")]
//{
        private double iOsMA(Mq4String symbol, int timeframe, int fast_ema_period, int slow_ema_period, int signal_period, int applied_price, int shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            var indicator = _cachedStandardIndicators.MacdCrossOver(series, fast_ema_period, slow_ema_period, signal_period);

            return indicator.Histogram.FromEnd(shift);
        }       
//}

		[Conditional("iCCI", "iCCIOnArray")]
		//{
#region iCCI
        private double iCCI(Mq4String symbol, int timeframe, int period, int applied_price, int shift)
        {
            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateCCI(series, period, shift);
        }       
                
        private double iCCIOnArray(Mq4OutputDataSeries invertedDataSeries, int period, int shift) 
        {            
            return CalculateCCI(invertedDataSeries.OutputDataSeries, period, shift);
        }
        
        private Dictionary<CciParameters, CciIndicator> _cciIndicators = new Dictionary<CciParameters, CciIndicator>();
        private double CalculateCCI(DataSeries dataSeries, int period, int shift)
        {     
            var parameters = new CciParameters
			{
                Source = dataSeries,
                Period = period
			};
            CciIndicator indicator = null;
            if (!_cciIndicators.TryGetValue(parameters, out indicator))
			{
                indicator = new CciIndicator(parameters, Indicators);
                _cciIndicators[parameters] = indicator;
			}
            
            var index = dataSeries.Count - 1 - shift;
            return indicator.Calculate(index);
        }

        class CciParameters
        {
            public DataSeries Source { get; set; }
            public int Period { get; set; }
        }

        class CciIndicator
        {
            private readonly CciParameters _parameters;
            private readonly SimpleMovingAverage _sma;

            public CciIndicator(CciParameters parameters, IIndicatorsAccessor indicatorsAccessor)
            {
                _parameters = parameters;
                _sma = indicatorsAccessor.SimpleMovingAverage(_parameters.Source, _parameters.Period);
            }

            public double Calculate(int index)
            {
                var meanDeviation = 0.0;
                for (var i = (index - _parameters.Period); i < index; i++)
                {
                    meanDeviation = meanDeviation + Math.Abs(_parameters.Source[i] - _sma.Result[index]);
                }
                meanDeviation /= _parameters.Period;
                return (_parameters.Source[index] - _sma.Result[index]) / (meanDeviation * 0.015);
            }
        }

        
#endregion //iCCI		
		//}

		[Conditional("iStdDev", "iStdDevOnArray")]
		//{
#region iStdDev
        private double iStdDev(Mq4String symbol, int timeframe, int ma_period, int ma_shift, int ma_method, int applied_price, int shift)
        {
            if (ma_shift != 0)
                throw new NotImplementedException(NotSupportedMaShift);

            var series = ToAppliedPrice(symbol, timeframe, applied_price);
      
            return CalculateiStdDev(series, ma_period, ma_shift, ma_method, shift);
        }       
                
        private double iStdDevOnArray(Mq4OutputDataSeries invertedDataSeries, int total, int ma_period, int ma_shift, int ma_method, int shift) 
        {
          return CalculateiStdDev(invertedDataSeries.OutputDataSeries, ma_period, ma_shift, ma_method, shift);
        }
        
        private double CalculateiStdDev(DataSeries dataSeries, int ma_period, int ma_shift, int ma_method, int shift)
        {     
            if (ma_shift != 0)
                throw new Exception(NotSupportedMaShift);

            var maType = ToMaType(ma_method);            
            var indicator = _cachedStandardIndicators.StandardDeviation(dataSeries, ma_period, maType);

            return indicator.Result.FromEnd(shift);
        }        
#endregion //iStdDev		
		//}

		[Conditional("iWPR")]
		//{
#region iWPR
        private double iWPR(Mq4String symbol, int timeframe, int period, int shift)
        {
			var marketSeries = GetSeries(symbol, timeframe);
            var indicator = _cachedStandardIndicators.WilliamsPctR(marketSeries, period);

            return indicator.Result.FromEnd(shift);
        }        
#endregion //iWPR		
		//}

		[Conditional("iSAR")]
		//{
#region iSAR
        private double iSAR(Mq4String symbol, int timeframe, double step, double maximum, int shift)
        {
			var series = GetSeries(symbol, timeframe);
            var indicator = _cachedStandardIndicators.ParabolicSAR(series, step, maximum);

            return indicator.Result.FromEnd(shift);
        }        
#endregion //iSAR
		//}

[Conditional("iFractals")]
//{
	private Mq4Double iFractals(Mq4String symbol, int timeframe, int mode, int shift)
	{
		var index = _currentIndex - shift;
		var marketSeries = GetSeries(symbol, timeframe);
		if (mode == MODE_UPPER)
		{
			if (IsUpFractal(marketSeries, index))
				return marketSeries.High[index];
		}
		if (mode == MODE_LOWER)
		{
			if (IsDownFractal(marketSeries, index))
				return marketSeries.Low[index];
		}
		return 0;
	}
		
	private static bool IsUpFractal(MarketSeries marketSeries, int i)
    {
        var high = marketSeries.High;
        if (high[i] > high[i - 1] && high[i] > high[i - 2] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] == high[i - 1] && high[i] > high[i - 2] && high[i] > high[i - 3] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] >= high[i - 1] && high[i] == high[i - 2] && high[i] > high[i - 3] && high[i] > high[i - 4] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] >= high[i - 1] && high[i] == high[i - 2] && high[i] == high[i - 3] && high[i] > high[i - 4] && high[i] > high[i - 5] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        if (high[i] >= high[i - 1] && high[i] == high[i - 2] && high[i] >= high[i - 3] && high[i] == high[i - 4] && high[i] > high[i - 5] && high[i] > high[i - 6] && high[i] > high[i + 1] && high[i] > high[i + 2])
            return true;

        return false;
    }

    private static bool IsDownFractal(MarketSeries marketSeries, int i)
    {
        var low = marketSeries.Low;
        if (low[i] < low[i - 1] && low[i] < low[i - 2] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] == low[i - 1] && low[i] < low[i - 2] && low[i] < low[i - 3] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] <= low[i - 1] && low[i] == low[i - 2] && low[i] < low[i - 3] && low[i] < low[i - 4] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] <= low[i - 1] && low[i] == low[i - 2] && low[i] == low[i - 3] && low[i] < low[i - 4] && low[i] < low[i - 5] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        if (low[i] <= low[i - 1] && low[i] == low[i - 2] && low[i] <= low[i - 3] && low[i] == low[i - 4] && low[i] < low[i - 5] && low[i] < low[i - 6] && low[i] < low[i + 1] && low[i] < low[i + 2])
            return true;

        return false;
    }

//}

[Conditional("iStochastic")]
//{

	Mq4Double iStochastic(Mq4String symbol, int timeframe, int kperiod, int dperiod, int slowing, int method, int price_field, int mode, int shift)
	{
		var maType = ToMaType(method);   
		var stochasticMode = method == 0 ? StochasticMode.LowHigh : StochasticMode.CloseClose;		
		var marketSeries = GetSeries(symbol, timeframe);
        var index = marketSeries.Close.Count - 1 - shift;

		var stochasticValues = CalculateStochastic(marketSeries, kperiod, dperiod, slowing, maType, stochasticMode, index);

		switch (mode)
		{
			case MODE_MAIN:
				return stochasticValues.K;
			case MODE_SIGNAL:
				return stochasticValues.D; 
		}

		return 0;
	}

	private struct StochasticParameters
    {
        public int KPeriods;
		public int DPeriods;
		public int KSlowing;
		public MovingAverageType MAType;
		public StochasticMode StochasticMode;
		public MarketSeries MarketSeries;
    }

	private class StochasticValues
	{
		public Mq4Double K { get; private set; }
		public Mq4Double D { get; private set; }

		public StochasticValues(double k, double d)
		{
			K = k;
			D = d;
		}
	}

	private enum StochasticMode
	{
		LowHigh,
		CloseClose
	}

	private class Mq4StochasticIndicator
	{
		private readonly StochasticParameters _parameters;
		private IndicatorDataSeries _fastK;
		private MovingAverage _slowK;
		private MovingAverage _averageOnSlowK;

		public Mq4StochasticIndicator(
			StochasticParameters stochasticParameters, 
			IIndicatorsAccessor indicatorAccessor, 
			Func<IndicatorDataSeries> dataSeriesFactory)
		{
			_parameters = stochasticParameters;
			_fastK = dataSeriesFactory();
			_slowK = indicatorAccessor.MovingAverage(_fastK, _parameters.KSlowing, _parameters.MAType);
			_averageOnSlowK = indicatorAccessor.MovingAverage(_slowK.Result, _parameters.DPeriods, _parameters.MAType);
		}

		public StochasticValues Calculate(int index)
        {
            for (var i = index - _parameters.KSlowing * _parameters.DPeriods; i <= index; i++)
				_fastK[i] = GetFastKValue(i);	

            for (var i = Math.Max(0, index - _parameters.DPeriods); i <= index; i++)
                _slowK.Calculate(i + 1);
            var k = _slowK.Result[index];

            for (var i = Math.Max(0, index - _parameters.DPeriods); i <= index; i++)
                _averageOnSlowK.Calculate(i + 1);    
            var d = _averageOnSlowK.Result[index];

            return new StochasticValues(k, d);
        }

		private double GetFastKValue(int index)
		{
			DataSeries low = _parameters.MarketSeries.Low;
			DataSeries high = _parameters.MarketSeries.High;
				
			if (_parameters.StochasticMode == StochasticMode.CloseClose)
			{
				low = _parameters.MarketSeries.Close;
				high = _parameters.MarketSeries.Close;
			}
			double minFromPeriod = GetMinFromPeriod(low, index, _parameters.KPeriods);
			double maxFromPeriod = GetMaxFromPeriod(high, index, _parameters.KPeriods);
			return (_parameters.MarketSeries.Close[index] - minFromPeriod) / (maxFromPeriod - minFromPeriod) * 100.0;
		}

		private static double GetMinFromPeriod(DataSeries dataSeries, int endIndex, int periods)
		{
			double num = dataSeries[endIndex];
			for (int index = endIndex; index > endIndex - periods; --index)
			{
				if (dataSeries[index] < num)
					num = dataSeries[index];
			}
			return num;
		}

		private static double GetMaxFromPeriod(DataSeries dataSeries, int endIndex, int periods)
		{
			double num = dataSeries[endIndex];
			for (int index = endIndex; index > endIndex - periods; --index)
			{
				if (dataSeries[index] > num)
					num = dataSeries[index];
			}
			return num;
		}
	}
    
    private Dictionary<StochasticParameters, Mq4StochasticIndicator> _stochasticIndicators = new Dictionary<StochasticParameters, Mq4StochasticIndicator>();

	private StochasticValues CalculateStochastic(MarketSeries marketSeries, int kPeriod, int dPeriod, int slowing, MovingAverageType maType, StochasticMode stochasticMode, int index)
	{
		var parameters = new StochasticParameters
		{
			KPeriods = kPeriod,
			DPeriods = dPeriod,
			KSlowing = slowing,
			MAType = maType,
			StochasticMode = stochasticMode,
			MarketSeries = marketSeries,
		};

		Mq4StochasticIndicator indicator;
		if (!_stochasticIndicators.TryGetValue(parameters, out indicator))
		{
			indicator = new Mq4StochasticIndicator(parameters, Indicators, () => CreateDataSeries());
			_stochasticIndicators[parameters] = indicator;
		}

		return indicator.Calculate(index);
	}	
//}
[Conditional("iBearsPower")]
Mq4Double iBearsPower(Mq4String symbol, int timeframe, int period, int applied_price, int shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
		
	return marketSeries.Low.FromEnd(shift) - iMA(symbol, timeframe, period, 0, MODE_EMA, applied_price, shift);
}

[Conditional("iBullsPower")]
Mq4Double iBullsPower(Mq4String symbol, int timeframe, int period, int applied_price, int shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
		
	return marketSeries.High.FromEnd(shift) - iMA(symbol, timeframe, period, 0, MODE_EMA, applied_price, shift);
}

[Conditional("iMomentum")]
Mq4Double iMomentum(Mq4String symbol, int timeframe, int period, int applied_price, int shift)
{
	var series = ToAppliedPrice(symbol, timeframe, applied_price);

	return _cachedStandardIndicators.MomentumOscillator(series, period).Result.FromEnd(shift);
}
[Conditional("iForce")]
//{
Mq4Double iForce(Mq4String symbol, int timeframe, int period, int ma_method, int applied_price, int shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
	return marketSeries.TickVolume.FromEnd(shift) * 
		(iMA(symbol, timeframe, period, 0, ma_method, applied_price, shift) - iMA(symbol, timeframe, period, 0, ma_method, applied_price, shift + 1));
}
//}
[Conditional("iRVI")]
//{
Mq4Double CalculateRvi(Mq4String symbol, int timeframe, int period, int shift)
{
	var marketSeries = GetSeries(symbol, timeframe);
	var close = marketSeries.Close;
	var open = marketSeries.Open;
	var high = marketSeries.High;
	var low = marketSeries.Low;
	
	var v1sum = 0d;
	var v2sum = 0d;
	for (var i = shift; i < shift + period; i++)
	{
		var v1 = ((close.FromEnd(i + 0) - open.FromEnd(i + 0)) + 2 * (close.FromEnd(i + 1) - open.FromEnd(i + 1)) + 2 * (close.FromEnd(i + 2) - open.FromEnd(i + 2)) + (close.FromEnd(i + 3) - open.FromEnd(i + 3))) / 6;
		var v2 = ((high.FromEnd(i + 0) - low.FromEnd(i + 0)) + 2 * (high.FromEnd(i + 1) - low.FromEnd(i + 1)) + 2 * (high.FromEnd(i + 2)- low.FromEnd(i + 2)) + (high.FromEnd(i + 3) - low.FromEnd(i + 3))) / 6;
		v1sum += v1;
		v2sum += v2;
	}

	return v1sum / v2sum;
}

Mq4Double iRVI(Mq4String symbol, int timeframe, int period, int mode, int shift)
{
	if (mode == MODE_MAIN)
		return CalculateRvi(symbol, timeframe, period, shift);

	return (CalculateRvi(symbol, timeframe, period, shift) + 2 * CalculateRvi(symbol, timeframe, period, shift + 1) + 2 * CalculateRvi(symbol, timeframe, period, shift + 2) +
		 CalculateRvi(symbol, timeframe, period, shift + 3)) / 6;
}
//}
[Conditional("iCustom")]
//{
internal class CustomIndicatorParameters
{
    public MarketSeries MarketSeries { get; set; }
    public object[] Parameters { get; set; }

    protected bool Equals(CustomIndicatorParameters other)
    {
        if (Parameters.Length != other.Parameters.Length)
            return false;
        for (var i = 0; i < Parameters.Length; i++)
        {
            if (!Equals(Parameters[i], other.Parameters[i]))
                return false;
        }

        return Equals(MarketSeries, other.MarketSeries);
    }

    public override bool Equals(object obj)
    {
        if (ReferenceEquals(null, obj)) return false;
        if (ReferenceEquals(this, obj)) return true;
        if (obj.GetType() != this.GetType()) return false;
        return Equals((CustomIndicatorParameters) obj);
    }

    public override int GetHashCode()
    {
        return (MarketSeries != null ? MarketSeries.GetHashCode() : 0);
    }
}

private List<DataSeries> GetAllOutputDataSeries(object indicatorInstance)
{
	var fieldInfo = indicatorInstance.GetType().GetField("AllOutputDataSeries");
    return (List<DataSeries>)fieldInfo.GetValue(indicatorInstance);
}

private object CastParameter(object parameter)
{
	if (parameter is Mq4Double)
	{
		var mq4Double = (Mq4Double)parameter;
		if (Math.Abs(mq4Double - (int)mq4Double) < Symbol.PointSize)
			return (int)mq4Double;
		return (double)mq4Double;
	}
	if (parameter is string || parameter is Mq4String)
	{
		return (string)parameter;
	}
	return parameter;
}

private object[] CastParameters<T>(object[] parameters)
{
	return parameters
		.Select(CastParameter)
		.ToArray();
}


private readonly Dictionary<CustomIndicatorParameters, List<DataSeries>> _customIndicatorsCache = new Dictionary<CustomIndicatorParameters, List<DataSeries>>();

Mq4Double iCustom<T>(Mq4String symbol, int timeframe, string name, params object[] parameters) where T : Indicator
{
	var marketSeries = GetSeries(symbol, timeframe);
	var indicatorParameters = CastParameters<T>(parameters.Take(parameters.Length - 2).ToArray());	
		
	var customIndicatorParameters = new CustomIndicatorParameters{ MarketSeries = marketSeries, Parameters = indicatorParameters};
	List<DataSeries> outputSeries;
	if (!_customIndicatorsCache.TryGetValue(customIndicatorParameters, out outputSeries))
	{
		var customIndicator = Indicators.GetIndicator<T>(marketSeries, indicatorParameters);	
		outputSeries = GetAllOutputDataSeries(customIndicator);
		_customIndicatorsCache[customIndicatorParameters] = outputSeries;
	}	
		
	var mode = (int)CastParameter(parameters[parameters.Length - 2]);
	var shift = (int)CastParameter(parameters[parameters.Length - 1]);
    return outputSeries[mode].FromEnd(shift);
}
//}