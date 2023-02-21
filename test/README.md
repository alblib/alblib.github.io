
# ParallelNumerics 

![Swift_5.7+](https://img.shields.io/static/v1?label=Swift&color=lightgreen&message=5.7%2B&style=flat&logo=Swift)
![SwiftPM](https://img.shields.io/badge/-SwiftPM-critical?logo=Swift&style=flat&logoColor=white)
![Swift-DocC](https://img.shields.io/badge/-DocC-1C5ED0?logo=Swift&style=flat&logoColor=white)
![Swift-DocC](https://img.shields.io/badge/-Apple%20Platform%20Only-gray?logo=Apple&style=flat&logoColor=white)


## Submodules

depends on
[![Accelerate](https://img.shields.io/badge/-Accelerate-gray?logoColor=white&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACoAAAApCAYAAABDV7v1AAAAAXNSR0IArs4c6QAAAERlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAKqADAAQAAAABAAAAKQAAAACTut/bAAADpUlEQVRYCe2XO0hWYRjH/bwlatmggy2JIjkUEkQO2iS6KY1WUESNQQ1BuVQ0VDQE0RIJgTZXtHShIsSGitqihrAGo4sNkkllF+33j/MeX1/Pe77vfDcdfODn97zP3XN7zykpWZUVegTm5+eb4ApMwhcYgqYVMy7DVMNp+AGuyCZf9bIOzAADMAHpRDEDRR+WplthLN10EX7lbC34wDRpAF2HfyFbUa5qNOR9YIqWwxGYgnyJaqlmeV4GplAvvIJCiWr3Zj0syS1wq1DTRdRVr5aMBya4DE7BTyi2qKd6l6UdmCA9qJdbhtxBU66BCb9iW+fai7yeTqVSdXbPUnsR6A8jbMU2pZ+BI1oDV5fx3Kt35lsuwZ3woogDq1dnVqeOxFI4CHojKpSotnpEXYbJ5qZIHVyAX5AvUS3VXHTTJJvME03RNrgLuYpqtHnaZGamwB44BN6HLr4+eANJRTl9vknwabNR792+mNBOkHnxeIneHTocBV8lHINvkE4Uo9hKp0y4xNcN6imZCh0+haD7/0MX/txEbY6Jb8Q/DHMLKaEmm3yNMfnN+NXDlge++NBOtD4tzsKslak9+AzUhoGOgq8DdO1pOHEPOpywcImvFtRHtY3oBjsPNWFgOoXgVrgDtnxgsReWbL2mHj79o94HtnJBNVTLFv1jm0ydxL8k98NbuyL6E/AeLV8T5QS5/ITyDm2nLyeRnUJVcAK+g5E5lNjrzzQhLuo6Vq2TUGXiEv2SGHdaN+K/Drbojh6ENW4j2QKf+2S4gb3JjTdrfN4ZTEwJQSMwCu2h0VHw9cBrsGWcRXgKpYNstiinxykXLvG1g3qPhEafQtAMSPS1eBnqo2KxV8BRmAZbHrMYsw3oilFshadWPT71Uk/JTFTcIhtBx8EkKEkbwGGI/FrEruvvGvhEvsjnKHZ93aq2ehjRtT+4aCjfgsBtoDvbltivRQK74LmVIL0rpkcvftW05RmL7b6cSDsJet7th89gi/drkSDl7AiIvCHwtYD7dTuJ7QBE5kQO6BpJroOL8BuMzKKcg7VuvG+t2CBHuUb+oFyC9b68xHaKbYZHYIt2l33gPRLyBTHuTjSKfUviQTJNoPgATIAtT1ks2alkA/lsec9iV6b9coqjkT4A3RcXs1NtwCeGQTYj5nLxvtjkNFRcMhPoxeW2mST41RuQsEUxrXG1iuJjCL24jNuTBbps/UUZItMmDKQXF20WH+FToGf3wpFp09W4HI7APy0RUhxi17rGAAAAAElFTkSuQmCC&style=flat)](https://developer.apple.com/documentation/accelerate)
[![swift-numerics](https://img.shields.io/badge/-swift--numerics-gray?logoColor=white&logo=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACoAAAApCAYAAABDV7v1AAAAAXNSR0IArs4c6QAAAERlWElmTU0AKgAAAAgAAYdpAAQAAAABAAAAGgAAAAAAA6ABAAMAAAABAAEAAKACAAQAAAABAAAAKqADAAQAAAABAAAAKQAAAACTut/bAAADpUlEQVRYCe2XO0hWYRjH/bwlatmggy2JIjkUEkQO2iS6KY1WUESNQQ1BuVQ0VDQE0RIJgTZXtHShIsSGitqihrAGo4sNkkllF+33j/MeX1/Pe77vfDcdfODn97zP3XN7zykpWZUVegTm5+eb4ApMwhcYgqYVMy7DVMNp+AGuyCZf9bIOzAADMAHpRDEDRR+WplthLN10EX7lbC34wDRpAF2HfyFbUa5qNOR9YIqWwxGYgnyJaqlmeV4GplAvvIJCiWr3Zj0syS1wq1DTRdRVr5aMBya4DE7BTyi2qKd6l6UdmCA9qJdbhtxBU66BCb9iW+fai7yeTqVSdXbPUnsR6A8jbMU2pZ+BI1oDV5fx3Kt35lsuwZ3woogDq1dnVqeOxFI4CHojKpSotnpEXYbJ5qZIHVyAX5AvUS3VXHTTJJvME03RNrgLuYpqtHnaZGamwB44BN6HLr4+eANJRTl9vknwabNR792+mNBOkHnxeIneHTocBV8lHINvkE4Uo9hKp0y4xNcN6imZCh0+haD7/0MX/txEbY6Jb8Q/DHMLKaEmm3yNMfnN+NXDlge++NBOtD4tzsKslak9+AzUhoGOgq8DdO1pOHEPOpywcImvFtRHtY3oBjsPNWFgOoXgVrgDtnxgsReWbL2mHj79o94HtnJBNVTLFv1jm0ydxL8k98NbuyL6E/AeLV8T5QS5/ITyDm2nLyeRnUJVcAK+g5E5lNjrzzQhLuo6Vq2TUGXiEv2SGHdaN+K/Drbojh6ENW4j2QKf+2S4gb3JjTdrfN4ZTEwJQSMwCu2h0VHw9cBrsGWcRXgKpYNstiinxykXLvG1g3qPhEafQtAMSPS1eBnqo2KxV8BRmAZbHrMYsw3oilFshadWPT71Uk/JTFTcIhtBx8EkKEkbwGGI/FrEruvvGvhEvsjnKHZ93aq2ehjRtT+4aCjfgsBtoDvbltivRQK74LmVIL0rpkcvftW05RmL7b6cSDsJet7th89gi/drkSDl7AiIvCHwtYD7dTuJ7QBE5kQO6BpJroOL8BuMzKKcg7VuvG+t2CBHuUb+oFyC9b68xHaKbYZHYIt2l33gPRLyBTHuTjSKfUviQTJNoPgATIAtT1ks2alkA/lsec9iV6b9coqjkT4A3RcXs1NtwCeGQTYj5nLxvtjkNFRcMhPoxeW2mST41RuQsEUxrXG1iuJjCL24jNuTBbps/UUZItMmDKQXF20WH+FToGf3wpFp09W4HI7APy0RUhxi17rGAAAAAElFTkSuQmCC&style=flat)](https://github.com/apple/swift-numerics)


<img src="./images/square-stack-3d-up-fill.svg" width="12" height="12"> `BigNumbers`
<br/>&nbsp;&nbsp;&nbsp;&nbsp;<sub>
*Big numerals with their operations using __Accelerate__*</sub>

<img src="./images/square-stack-3d-up-fill.svg" width="12" height="12"> `LinearAlgebra`
<br/>&nbsp;&nbsp;&nbsp;&nbsp;<sub>
*Real vectors and matrices with their operations using __Accelerate__*</sub>

<img src="./images/square-stack-3d-up-fill.svg" width="12" height="12"> `ComplexLinearAlgebra`
<br/>&nbsp;&nbsp;&nbsp;&nbsp;<sub>
*Complex vectors and matrices with their operations using __Accelerate__*</sub>

<img src="./images/square-stack-3d-up-fill.svg" width="12" height="12"> `Polynomials`
<br/>&nbsp;&nbsp;&nbsp;&nbsp;<sub>
*Univariate and multivariate polynomials with their operations using __Accelerate__*</sub>

<img src="./images/square-stack-3d-up-fill.svg" width="12" height="12"> `SignalProcessing`
<br/>&nbsp;&nbsp;&nbsp;&nbsp;<sub>
*Digital signal processing using FIR and IIR Filters represented by digital transfer function, with bilinear transform and discrete Fourier transform, defined in __Accelerate.vDSP__*</sub>

<img src="./images/square-stack-3d-up-fill.svg" width="12" height="12"> `PlotInterpolations`
<br/>&nbsp;&nbsp;&nbsp;&nbsp;<sub>
*Support various interpolation methods between plot points using Hermite cubic splines.</sub>
